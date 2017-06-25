#include <RcppArmadillo.h>
using namespace Rcpp;
using namespace arma;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//
// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::export]]
double Schott2007Stat(List x) {
  int len = x.length();
  arma::mat pmat = x[1];
  arma::vec a2i(len);
  List samplecov(len);
  double p = pmat.n_cols;
  double ntot = 0;
  arma::mat Apool(p, p);
  Apool.fill(0);
  double ninv = 0;

 for(int i = 0; i < len; ++i){
    arma::mat mats = x[i];
    double ns = mats.n_rows;
    int ps = mats.n_cols;

    arma::mat covar = cov(mats);
    samplecov[i] = covar;

    a2i[i] = pow(ns - 1, 2) *
      pow(ps * (ns - 2) * (ns + 1), -1)*
      (trace(covar * covar) - pow(ns - 1, -1) * pow(trace(covar), 2));

    Apool += covar * ns;
    ntot += ns - 1;
    ninv += pow(ns - 1, -1);
  }

 arma::mat pooledCov = Apool * pow(ntot, -1);

 double a2 = pow(ntot, 2) *
   pow(p * (ntot - 1) * (ntot + 2), -1) *
   (trace(pooledCov * pooledCov) - pow(ntot, -1) * pow(trace(pooledCov), 2));

  double theta = 0;

  for(int i = 0; i < len; ++i){
    arma::mat mats = x[i];
    int ns = mats.n_rows;
  theta += 2 * a2 * ninv;
  }

  double stat = 0;
  for(int i = 0; i < len; ++i){
    arma::mat sampcovi = samplecov[i];
    double ai = a2i[i];
    for(int j = i + 1; j < len; ++j){
      double aj = a2i[j];
      arma::mat sampcovj = samplecov[j];
    stat += pow(pow(theta, -1) *  (ai + aj - 2 * pow(p, -1) * trace(sampcovi * sampcovj)), 2);
    }
  }

  return stat;
}


// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//
// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::export]]
double Schott2007pooledStat(List x) {
  int len = x.length();
  arma::mat pmat = x[1];
  arma::vec a2i(len);
  List samplecov(len);
  double p = pmat.n_cols;
  double ntot = 0;
  arma::mat Apool(p, p);
  Apool.fill(0);
  double ninv = 0;

  for(int i = 0; i < len; ++i){
    arma::mat mats = x[i];
    double ns = mats.n_rows;
    int ps = mats.n_cols;

    arma::mat covar = cov(mats);
    samplecov[i] = covar;

    a2i[i] = pow(ns - 1, 2) *
      pow(ps * (ns - 2) * (ns + 1), -1)*
      (trace(covar * covar) - pow(ns - 1, -1) * pow(trace(covar), 2));

    Apool += covar * ns;
    ntot += ns - 1;
    ninv += pow(ns - 1, -1);
  }

  arma::mat pooledCov = Apool * pow(ntot, -1);

  double a2 = pow(ntot, 2) *
    pow(p * (ntot - 1) * (ntot + 2), -1) *
    (trace(pooledCov * pooledCov) - pow(ntot, -1) * pow(trace(pooledCov), 2));

  double theta = 0;

  for(int i = 0; i < len; ++i){
    arma::mat mats = x[i];
    int ns = mats.n_rows;
    theta += 2 * a2 * ninv;
  }

  double stat = 0;
  for(int i = 0; i < len; ++i){
    arma::mat sampcovi = samplecov[i];
    double ai = a2i[i];

    stat += pow(pow(theta, -1) *  (ai + a2 - 2 * pow(p, -1) * trace(sampcovi * pooledCov)), 2);
  }

  return stat;
}

