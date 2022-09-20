#pragma once

#include "map.h"
#include "set.h"
#include <string>

// Prototypes to be shared with other modules

std::string cleanToken(std::string token);

Set<std::string> gatherTokens(std::string bodyText);

int buildIndex(std::string dbfile, Map<std::string, Set<std::string>>& index);

Set<std::string> findQueryMatches(Map<std::string, Set<std::string>>& index, std::string query);

void searchEngine(std::string dbfile);
