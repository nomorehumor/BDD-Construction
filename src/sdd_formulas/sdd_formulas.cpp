//
// Created by Maxim.Popov on 04.07.2022.
//

#include "sdd_formulas.h"
#include "../stats/BDDBuildStatistic.h"
#include "../utils/configuration.h"
#include "../utils/output_utils.h"
#include "spdlog/spdlog.h"

namespace sdd {
    
    SddNode* createFormulaSdd(SddManager *sm, FormulaInfo info) {
        if (info.type == Form::AMO) {
            info = transformAMOtoDNF(info);
        }
        if (Configuration::getConstructionFormulaOrdering() == "dfs") {
            return createNFFormulaFromClauses(sm, info);
        } else if (Configuration::getConstructionFormulaOrdering() ==
                "merge") {
            return createNFFormulaMerge(sm, info);
        } else {
            return createNFFormulaFromClauses(sm, info);
        }
    
    }

    SddNode *createCNFClause(SddManager *sm, std::vector<int> clauseVars) {
        SddNode *clause = sdd_manager_false(sm);
        bool clauseSet = false;
        SddNode *tmpVar, *tmpClause;

        for (int var : clauseVars) {
            tmpVar = sdd_manager_literal((SddLiteral) var, sm);
            tmpClause = sdd_disjoin(tmpVar, clause, sm);
            sdd_ref(tmpClause, sm); 
            if (clauseSet) sdd_deref(clause, sm);
            else clauseSet = true;
            clause = tmpClause;
        }
        return clause;
    }

    SddNode *createDNFClause(SddManager *sm, std::vector<int> clauseVars) {
        SddNode *clause = sdd_manager_true(sm);
        bool clauseSet = false;
        SddNode *tmpVar, *tmpClause;

        for (int var : clauseVars) {
            tmpVar = sdd_manager_literal(var, sm);
            tmpClause = sdd_conjoin(tmpVar, clause, sm);
            sdd_ref(tmpClause, sm); 
            if (clauseSet) sdd_deref(clause, sm);
            else clauseSet = true;
            clause = tmpClause;
        }
        return clause;
    }

    SddNode *createNFFormulaFromClauses(SddManager *sm, FormulaInfo info) {
        SddNode *clauseSdd, *tmpClause, *sdd;

        if (info.type == Form::DNF) {
            sdd = sdd_manager_false(sm);
        } else if (info.type == Form::CNF) {
            sdd = sdd_manager_true(sm);
        }
        bool sddSet = false;

        int i = 0;
        for (std::vector<int> clause : info.clauses) {
            if (info.type == Form::DNF) {
                clauseSdd = createDNFClause(sm, clause);
            } else if (info.type == Form::CNF) {
                clauseSdd = createCNFClause(sm, clause);
            }

            if (info.type == Form::DNF) {
                tmpClause = sdd_disjoin(clauseSdd, sdd, sm);
            } else if (info.type == Form::CNF) {
                tmpClause = sdd_conjoin(clauseSdd, sdd, sm);
            }
            sdd_ref(tmpClause, sm);
            sdd_deref(clauseSdd, sm);
            if (sddSet) sdd_deref(sdd, sm);
            else sddSet = true;
            sdd = tmpClause;
        }
        return sdd;
    }

    SddNode *createNFFormulaMerge(SddManager *sm, FormulaInfo info) {
        if (info.clauses.size() == 0) {
            SddNode *constant;
            if (info.type == Form::DNF) {
                constant = sdd_manager_false(sm);
            } else if (info.type == Form::CNF) {
                constant = sdd_manager_true(sm);
            }
            sdd_ref(constant, sm);
            return constant;
        } else if (info.clauses.size() == 1) {
            return info.type == Form::DNF ? createDNFClause(sm, info.clauses[0])
                                        : createCNFClause(sm, info.clauses[0]);
        } else {
            FormulaInfo formulaHalf1 = info;
            FormulaInfo formulaHalf2 = info;

            formulaHalf1.clauses = std::vector<std::vector<int>>(
                info.clauses.begin(),
                info.clauses.begin() + info.clauses.size() / 2);
            formulaHalf2.clauses = std::vector<std::vector<int>>(
                info.clauses.begin() + (info.clauses.size() / 2),
                info.clauses.end());

            SddNode *firstHalf = createNFFormulaMerge(sm, formulaHalf1);
            SddNode *secondHalf = createNFFormulaMerge(sm, formulaHalf2);

            SddNode *sdd = info.type == Form::DNF
                            ? sdd_disjoin(firstHalf, secondHalf, sm)
                            : sdd_conjoin(firstHalf, secondHalf, sm);
            sdd_ref(sdd, sm);
            sdd_deref(firstHalf, sm);
            sdd_deref(secondHalf, sm);

            return sdd;
        }
    }

    SddNode *createNFFormulaFromInfo(SddManager *sm, FormulaInfo info) {
        SddNode *lastVariables, *tmpLastVariables, *tmpVar, *sdd;
        lastVariables = NULL;
        if (info.type == Form::DNF) {
            sdd = sdd_manager_false(sm);
            lastVariables = sdd_manager_true(sm);
        } else if (info.type == Form::CNF) {
            sdd = sdd_manager_true(sm);
            lastVariables = sdd_manager_false(sm);
        }
        bool lastVariablesSet = false;

        for (int i = 0; i < info.symbols.size(); i++) {
            if (info.symbols.at(i) == 0 && (info.symbols.at(i - 1) != 0)) {
                if (info.type == Form::DNF) {
                    tmpVar = sdd_disjoin(lastVariables, sdd, sm);
                    sdd_ref(tmpVar, sm);
                    sdd_deref(lastVariables, sm);
                    lastVariables = sdd_manager_true(sm);
                } else if (info.type == Form::CNF) {
                    tmpVar = sdd_conjoin(lastVariables, sdd, sm);
                    sdd_ref(tmpVar, sm);
                    sdd_deref(lastVariables, sm);
                    lastVariables = sdd_manager_false(sm);
                }
                lastVariablesSet = false;
                sdd_deref(sdd, sm);
                sdd = tmpVar;
            } else if (info.symbols.at(i) != 0) {
                tmpVar = sdd_manager_literal(info.symbols.at(i), sm);

                if (info.type == Form::DNF) {
                    tmpLastVariables = sdd_conjoin(tmpVar, lastVariables, sm);
                } else if (info.type == Form::CNF) {
                    tmpLastVariables = sdd_disjoin(tmpVar, lastVariables, sm);
                }
                sdd_ref(tmpLastVariables, sm);
                if (lastVariablesSet) sdd_deref(lastVariables, sm);
                else lastVariablesSet = true;
                lastVariables = tmpLastVariables;
            }
        }

        sdd_deref(lastVariables, sm);

        return sdd;
    }
}