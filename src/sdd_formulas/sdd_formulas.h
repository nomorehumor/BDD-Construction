#include <vector>
#include "../utils/info_structs.h"
#include "sddapi.h"

namespace sdd {
    SddNode* createNFFormulaFromInfo(SddManager *manager, FormulaInfo info);
    SddNode* createNFFormulaFromClauses(SddManager *manager, FormulaInfo info);
    SddNode* createNFFormulaMerge(SddManager *manager, FormulaInfo info);
    SddNode* createAMOFormulaFromInfo(SddManager *manager, FormulaInfo amoInfo);
    SddNode* createFormulaSdd(SddManager *manager, FormulaInfo info);
}
