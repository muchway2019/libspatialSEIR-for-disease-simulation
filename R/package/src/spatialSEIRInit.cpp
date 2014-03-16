#include <Rcpp.h>
#include <cmath>
#include <ModelContext.hpp>

using namespace Rcpp;
using namespace SpatialSEIR;
// [[Rcpp::export]]
SEXP spatialSEIRInit(SEXP compMatDim,
                     SEXP xDim,
                     SEXP zDim,
                     SEXP S0_,
                     SEXP E0_,
                     SEXP I0_,
                     SEXP R0_,
                     SEXP Sstar0,
                     SEXP Estar0,
                     SEXP Istar0,
                     SEXP Rstar0,
                     SEXP Sstar, 
                     SEXP Estar, 
                     SEXP Istar, 
                     SEXP Rstar, 
                     SEXP X_,
                     SEXP Z_)
{

    //Deal with the data conversion from R to c++
    Rcpp::IntegerVector compartmentDimensions(compMatDim);
    Rcpp::IntegerVector covariateDimensions_x(xDim);
    Rcpp::IntegerVector covariateDimensions_z(zDim);
    Rcpp::IntegerVector S0(S0_);
    Rcpp::IntegerVector E0(E0_);
    Rcpp::IntegerVector I0(I0_);
    Rcpp::IntegerVector R0(R0_);

    Rcpp::IntegerVector S_star0(Sstar0);
    Rcpp::IntegerVector E_star0(Estar0);
    Rcpp::IntegerVector I_star0(Istar0);
    Rcpp::IntegerVector R_star0(Rstar0);

    Rcpp::IntegerVector S_star(Sstar);
    Rcpp::IntegerVector E_star(Estar);
    Rcpp::IntegerVector I_star(Istar);
    Rcpp::IntegerVector R_star(Rstar);



    Rcpp::NumericVector X(X_);
    Rcpp::NumericVector Z(Z_);

    // Create the empty ModelContext object 
    
    ModelContext* context = new ModelContext();

    // Create the covariate matrix object. 
    CovariateMatrix *CovMat = new CovariateMatrix();
    CovMat -> genFromDataStream(X.begin(), 
                                Z.begin(),
                                &covariateDimensions_x[0], 
                                &covariateDimensions_x[1],
                                &covariateDimensions_z[0], 
                                &covariateDimensions_z[1]);

    // Populate the CompartmentalModelMatrix objects. 
    
    context -> S_star -> genFromDataStream(S_star.begin(), 
                                           &compartmentDimensions[0],
                                           &compartmentDimensions[1]);
    context -> E_star -> genFromDataStream(E_star.begin(), 
                                           &compartmentDimensions[0],
                                           &compartmentDimensions[1]);
    context -> I_star -> genFromDataStream(I_star.begin(), 
                                           &compartmentDimensions[0],
                                           &compartmentDimensions[1]);
    context -> R_star -> genFromDataStream(R_star.begin(), 
                                           &compartmentDimensions[0],
                                           &compartmentDimensions[1]);


    CompartmentalModelMatrix *CompMat = new CompartmentalModelMatrix();
    CompMat -> genFromDataStream(I_star.begin(), 
                                 &compartmentDimensions[0],
                                 &compartmentDimensions[1]);


    context -> A0 ->  populate(S0.begin(),E0.begin(),I0.begin(),R0.begin(),
                               S_star0.begin(),E_star0.begin(),I_star0.begin(),
                               R_star0.begin(),&compartmentDimensions[1]);



    Rcpp::IntegerVector y = Rcpp::IntegerVector::create(0);
    //List z = List::create(y);
    Rcpp::XPtr<CompartmentalModelMatrix*> ptr(&CompMat, true);


    // Clean up

    Rcpp::Rcout << "Finished.\n";
    return ptr;
}
