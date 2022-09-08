extern "C" {
    #include "sddapi.h"
}
#include "../utils/info_structs.h"
#include "../stats/BDDBuildStatistic.h"

namespace sdd {
    SddNode* createRuleset(SddManager* sm, RulesetInfo& info, bool progress_output, BDDBuildStatistic* statistic=nullptr);
}