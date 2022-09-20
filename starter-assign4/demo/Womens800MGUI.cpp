#include "ProblemHandler.h"
#include "../pqclient.h"
#include "GUIUtils.h"
#include "CSV.h"
#include "TemporaryComponent.h"
#include "ginteractors.h"
#include "hashmap.h"
#include "vector.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <fstream>
#include "strlib.h"
#include "filelib.h"
#include "set.h"
using namespace std;

namespace {
    const string kBaseDirectory = "res/";
    const int kNumDisplayedSwimmers = 16;

    /* Background and foreground colors are designed to look like water. */
    const string kBackgroundColor = "#007FFF"; // Azure
    const string kForegroundColor = "#E1EBEE"; // Marian Blue

    /* General text description. */
    const string kDescription =
        "This visualization shows the progression in times for the Women's 800m Freestyle "
        "swimming event over time. As you drag the slider at the bottom of the window, you "
        "can see the " + to_string(kNumDisplayedSwimmers) + " fastest times recorded up to "
        "the given year at a collection of major sporting events. You may recognize some "
        "of the names that come up in this list!\n"
        "Data is taken from the International Olympic Committee and FINA.";
    const string kDescriptionFont = "SansSerif-BOLD-16";
    const string kDescriptionFontColor = kForegroundColor;
    const double kDescriptionHeight = 225;

    const double kWindowPadding = 15;
    const double kResultsPadding = 5;

    /* Graphics information for displaying results. */
    const string kGoldColor   = "#FFD700";
    const string kSilverColor = "#C0C0C0";
    const string kBronzeColor = "#CD7F32";
    const string kOtherColor  = "#555D50"; // Ebony

    const string kResultColor = kForegroundColor;
    const string kBorderColor = kForegroundColor;
    const string kResultFont  = "SansSerif-20";

    /* Type representing a race time. */
    struct RaceTime {
        int minutes;
        int seconds;
        int hundredths;
    };

    /* Type representing a swimmer's time. */
    struct SwimResult {
        int year;
        std::string event;
        std::string swimmer;
        std::string country;
        RaceTime time;
    };

    /* Given a string of the form MM:SS.HH, parses it into minutes, seconds, and hundredths. */
    RaceTime parseTime(const string& time) {
        istringstream extractor(time);
        char colon, dot;

        RaceTime result;
        extractor >> result.minutes >> colon >> result.seconds >> dot >> result.hundredths;

        if (!extractor || colon != ':' || dot != '.') error("Invalid time: " + time);

        char leftover;
        if (extractor >> leftover) error("Unexpected character: " + string(1, leftover));

        return result;
    }

    /* Give a base directory, returns all the swim records from the CSV files in that
     * directory.
     */
    Vector<SwimResult> parseCSVsIn(const string& baseDir) {
        Vector<SwimResult> allData;

        /* Pull up all CSV files from the base directory. */
        for (string filename: listDirectory(baseDir)) {
            /* If this isn't a CSV file, skip it. */
            if (!endsWith(filename, ".800m.csv")) continue;

            /* Otherwise, pull our data. */
            CSV data = CSV::parseFile(baseDir + filename);

            Vector<SwimResult> result;
            for (size_t row = 0; row < data.numRows(); row++) {
                result.add({
                    stringToInteger(data[row]["Year"]),
                    data[row]["Event"],
                    data[row]["Athlete"],
                    data[row]["Country"],
                    parseTime(data[row]["Time"])
                });
            }

            allData += result;
        }
        if (allData.isEmpty()) {
            error("No swim data files found in directory " + baseDir);
        }
        return allData;
    }

    Vector<SwimResult> loadData(const string& baseDir) {
        /* All the data points we have. */
        Vector<SwimResult> allData = parseCSVsIn(baseDir);

        /* We how have a bunch of time series. We can sort them by year by
         * using pqSort (key = index, value = year).
         */
        Vector<DataPoint> byYear(allData.size());
        for (int i = 0; i < allData.size(); i++) {
            byYear[i] = { to_string(i), double(allData[i].year) };
        }
        pqSort(byYear);
        auto result = allData;
        for (int i = 0; i < byYear.size(); i++) {
            if (!stringIsInteger(byYear[i].label) || stringToInteger(byYear[i].label) >= byYear.size()) {
                ostringstream out;
                out << "pqSort result contains erroneous DataPoint " << byYear[i];
                error(out.str());
            }
            result[i] = allData[stringToInteger(byYear[i].label)];
        }
        return result;
    }

    /* Given a RaceTime, expresses that RaceTime in seconds. */
    double toSeconds(const RaceTime& time) {
        return 60 * time.minutes + time.seconds + time.hundredths/100.0;
    }

    /* Comparison function for use in lower_bound. */
    struct YearComp {
        bool operator()(const DataPoint& lhs, const DataPoint& rhs) {
            return lhs.priority < rhs.priority;
        }
    };

    /* Problem handler that lets the user progressions in the Women's 800m Freestyle
     * swimming event at a collection of sporting events.
     */
    class Womens800MGUI: public ProblemHandler {
    public:
        Womens800MGUI(GWindow& window);

        void changeOccurredIn(GObservable* source) override;

    protected:
        void repaint(GWindow& window) override;

    private:
        /* Graphics components. */
        Temporary<GSlider> mYearSlider;

        /* Actively-displayed swim results. */
        Vector<SwimResult> mShown;

        /* Master data set, sorted by year. */
        Vector<SwimResult> mResults;

        /* Data points corresponding to the swim results. prioritys are years, keys are
         * indices into mResults.
         */
        Vector<DataPoint> mYearPoints;

        /* Data points corresponding to the swim times. prioritys are times measured in seconds,
         * keys are indices into mResults.
         */
        Vector<DataPoint> mTimePoints;

        /* Currently-displayed year. */
        int mYear = -1;

        /* Updates the display to show the best results up through a given year. */
        void showBestTimesThrough(int year);
    };

    Womens800MGUI::Womens800MGUI(GWindow& window) {
        /* Assemble our data sets. */
        mResults = loadData(kBaseDirectory);

        for (int i = 0; i < mResults.size(); i++) {
            mYearPoints.add({ to_string(i), double(mResults[i].year) });

            /* Notice that we store the negated times, which means that lower times are
             * considered better than higher times.
             */
            mTimePoints.add({ to_string(i), -toSeconds(mResults[i].time) });
        }

        mYearSlider = makeYearSlider(window, mResults[0].year, mResults[mResults.size()-1].year);

        /* Update the display. */
        showBestTimesThrough(mResults[0].year);
    }

    void Womens800MGUI::changeOccurredIn(GObservable* source) {
        if (source == mYearSlider) {
            showBestTimesThrough(mYearSlider->getValue());
        }
    }

    void Womens800MGUI::showBestTimesThrough(int year) {
        /* If the year hasn't changed, don't do anything. */
        if (year == mYear) return;
        mYear = year;

        /* Find the first event that occurs after the indicated year. That will be our
         * stop point.
         */
        double endPoint = lower_bound(mYearPoints.begin(), mYearPoints.end(), DataPoint{ "", double(year + 1) }, YearComp()) - mYearPoints.begin();

        stringstream stream;
        for (int i = 0; i < endPoint; i++) {
            stream << mTimePoints[i];
        }

        /* Find the winners. */
        mShown.clear();
        for (auto point: topK(stream, kNumDisplayedSwimmers)) {
            if (!stringIsInteger(point.label) || stringToInteger(point.label) >= mResults.size()) {
                ostringstream out;
                out << "TopK result contains erroneous DataPoint " << point;
                error(out.str());
            }
            mShown += mResults[stringToInteger(point.label)];
        }
        requestRepaint();
    }

    void Womens800MGUI::repaint(GWindow& window) {
        /* Clear the display. */
        clearDisplay(window, kBackgroundColor);

        /* Draw header text. */
        auto header = TextRender::construct(kDescription + "\n\nYear: " + to_string(mYear), {
                                                kWindowPadding, kWindowPadding,
                                                window.getCanvasWidth() - 2 * kWindowPadding,
                                                kDescriptionHeight - 2 * kWindowPadding
                                            }, kDescriptionFontColor, kDescriptionFont);
        header->draw(window);

        /* Assemble the list of results to display. */
        vector<string> swimmerList;
        for (auto result: mShown) {
            ostringstream builder;
            builder << result.year << ": "
                    << setfill('0') << setw(2) << result.time.minutes << ":"
                    << setw(2) << result.time.seconds << "."
                    << setw(2) << result.time.hundredths
                    << " by " << result.swimmer << " (" << result.country << ")"
                    << " at the " << result.event << endl;
            swimmerList.push_back(builder.str());
        }

        /* Assemble a list of colors for the markers. */
        vector<string> colorList = { kGoldColor, kSilverColor, kBronzeColor };
        while (colorList.size() < swimmerList.size()) {
            colorList.push_back(kOtherColor);
        }

        /* Draw the result. */
        auto bounds = header->bounds();
        double baseY = bounds.y + bounds.height + kResultsPadding;
        LegendRender::construct(swimmerList, colorList, {
                                    bounds.x, baseY,
                                    bounds.width, window.getCanvasHeight() - baseY - kResultsPadding
                                }, kResultColor, kResultFont, kBorderColor)->draw(window);
    }
}

shared_ptr<ProblemHandler> makeWomens800mGUI(GWindow& window) {
    return make_shared<Womens800MGUI>(window);
}
