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
        if (Configuration::getConstructionFormulaOrdering() == "dfs") {
            return createNFFormulaFromInfo(sm, info);
        } else if (Configuration::getConstructionFormulaOrdering() ==
                "merge") {
            return createNFFormulaMerge(sm, info);
        } else {
            return createNFFormulaFromInfo(sm, info);
        }
    
    }

    // SddNode *createAMOFormulaFromInfo(SddManager *sm, FormulaInfo amoInfo) {
    //     SddNode *tmp, *tmpVar;
    //     SddNode *bdd = sdd_manager_false(sm);
    //     sdd_ref(bdd);
    //     SddNode *notTerm = sdd_manager_true(sm);
    //     sdd_ref(notTerm);
    //     SddNode *zeroTerm = sdd_manager_false(sm);
    //     sdd_ref(zeroTerm);

    //     for (int i = 0; i < amoInfo.symbols.size(); i++) {
    //         if (amoInfo.symbols.at(i) == 0)
    //             continue;
    //         tmpVar = sdd_manager_literal(amoInfo.symbols.at(i), sm);

    //         tmp = sdd_(tmpVar, bdd, sm);
    //         sdd_ref(tmp);
    //         sdd_deref(bdd, sm);
    //         bdd = tmp;

    //         tmp = sdd_conjoin(tmpVar, notTerm, sm);
    //         sdd_ref(tmp);
    //         sdd_deref(notTerm, sm);
    //         notTerm = tmp;

    //         tmp = sdd_disjoin(tmpVar, zeroTerm, sm);
    //         sdd_ref(tmp);
    //         sdd_deref(zeroTerm, sm);
    //         zeroTerm = tmp;
    //     }

    //     notTerm = sdd_negate(notTerm, sm);
    //     tmp = sdd_conjoin(bdd, notTerm, sm);
    //     sdd_ref(tmp);
    //     sdd_deref(bdd, sm);
    //     sdd_deref(notTerm, sm);
    //     bdd = tmp;

    //     zeroTerm = sdd_negate(zeroTer, sm);
    //     tmp = sdd_disjoin(bdd, zeroTerm, sm);
    //     sdd_ref(tmp);
    //     sdd_deref(bdd, sm);
    //     sdd_deref(zeroTerm, sm);
    //     bdd = tmp;

    //     return bdd;
    // }

    SddNode *createCNFClause(SddManager *sm, std::vector<int> clauseVars) {
        SddNode *clause = sdd_manager_false(sm);
        SddNode *tmpVar, *tmpClause;

        for (int var : clauseVars) {
            tmpVar = sdd_manager_literal((SddLiteral) var, sm);
            tmpClause = sdd_disjoin(tmpVar, clause, sm);
            sdd_ref(tmpClause, sm); sdd_deref(clause, sm);
            clause = tmpClause;
        }
        return clause;
    }

    SddNode *createDNFClause(SddManager *sm, std::vector<int> clauseVars) {
        SddNode *clause = sdd_manager_true(sm);
        sdd_ref(clause, sm);
        SddNode *tmpVar, *tmpClause;

        for (int var : clauseVars) {
            tmpVar = sdd_manager_literal(var, sm);
            tmpClause = sdd_conjoin(tmpVar, clause, sm);
            sdd_ref(tmpClause, sm); sdd_deref(clause, sm);
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
        sdd_ref(sdd, sm);

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
            sdd_deref(sdd, sm);
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
        sdd_ref(sdd, sm);
        sdd_ref(lastVariables, sm);

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
                sdd_ref(lastVariables, sm);
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
                sdd_deref(lastVariables, sm);
                lastVariables = tmpLastVariables;
            }
        }

        sdd_deref(lastVariables, sm);

        return sdd;
    }
}