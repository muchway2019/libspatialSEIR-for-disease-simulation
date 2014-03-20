// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// rcppTestEntry
List rcppTestEntry();
RcppExport SEXP spatialSEIR_rcppTestEntry() {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        List __result = rcppTestEntry();
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
// spatialSEIRInit
SEXP spatialSEIRInit(SEXP compMatDim, SEXP xDim, SEXP zDim, SEXP S0_, SEXP E0_, SEXP I0_, SEXP R0_, SEXP Sstar0, SEXP Estar0, SEXP Istar0, SEXP Rstar0, SEXP Sstar, SEXP Estar, SEXP Istar, SEXP Rstar, SEXP X_, SEXP Z_, SEXP DistMat_, SEXP rho_, SEXP beta_, SEXP p_ei_, SEXP p_ir_, SEXP p_rs_, SEXP N_);
RcppExport SEXP spatialSEIR_spatialSEIRInit(SEXP compMatDimSEXP, SEXP xDimSEXP, SEXP zDimSEXP, SEXP S0_SEXP, SEXP E0_SEXP, SEXP I0_SEXP, SEXP R0_SEXP, SEXP Sstar0SEXP, SEXP Estar0SEXP, SEXP Istar0SEXP, SEXP Rstar0SEXP, SEXP SstarSEXP, SEXP EstarSEXP, SEXP IstarSEXP, SEXP RstarSEXP, SEXP X_SEXP, SEXP Z_SEXP, SEXP DistMat_SEXP, SEXP rho_SEXP, SEXP beta_SEXP, SEXP p_ei_SEXP, SEXP p_ir_SEXP, SEXP p_rs_SEXP, SEXP N_SEXP) {
BEGIN_RCPP
    SEXP __sexp_result;
    {
        Rcpp::RNGScope __rngScope;
        Rcpp::traits::input_parameter< SEXP >::type compMatDim(compMatDimSEXP );
        Rcpp::traits::input_parameter< SEXP >::type xDim(xDimSEXP );
        Rcpp::traits::input_parameter< SEXP >::type zDim(zDimSEXP );
        Rcpp::traits::input_parameter< SEXP >::type S0_(S0_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type E0_(E0_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type I0_(I0_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type R0_(R0_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type Sstar0(Sstar0SEXP );
        Rcpp::traits::input_parameter< SEXP >::type Estar0(Estar0SEXP );
        Rcpp::traits::input_parameter< SEXP >::type Istar0(Istar0SEXP );
        Rcpp::traits::input_parameter< SEXP >::type Rstar0(Rstar0SEXP );
        Rcpp::traits::input_parameter< SEXP >::type Sstar(SstarSEXP );
        Rcpp::traits::input_parameter< SEXP >::type Estar(EstarSEXP );
        Rcpp::traits::input_parameter< SEXP >::type Istar(IstarSEXP );
        Rcpp::traits::input_parameter< SEXP >::type Rstar(RstarSEXP );
        Rcpp::traits::input_parameter< SEXP >::type X_(X_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type Z_(Z_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type DistMat_(DistMat_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type rho_(rho_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type beta_(beta_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type p_ei_(p_ei_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type p_ir_(p_ir_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type p_rs_(p_rs_SEXP );
        Rcpp::traits::input_parameter< SEXP >::type N_(N_SEXP );
        SEXP __result = spatialSEIRInit(compMatDim, xDim, zDim, S0_, E0_, I0_, R0_, Sstar0, Estar0, Istar0, Rstar0, Sstar, Estar, Istar, Rstar, X_, Z_, DistMat_, rho_, beta_, p_ei_, p_ir_, p_rs_, N_);
        PROTECT(__sexp_result = Rcpp::wrap(__result));
    }
    UNPROTECT(1);
    return __sexp_result;
END_RCPP
}
