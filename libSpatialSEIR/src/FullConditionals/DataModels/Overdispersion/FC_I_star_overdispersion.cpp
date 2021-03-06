#include<math.h>
#include<cstring>
#include<vector>
#include<cmath>
#include<algorithm>
#include<LSS_Samplers.hpp>
#include<LSS_FC_I_star_overdispersion.hpp>
#include<ModelContext.hpp>
#include<OCLProvider.hpp>
#include<CompartmentalModelMatrix.hpp>
#include<CovariateMatrix.hpp>
#include<RandomNumberProvider.hpp>
#include<IOProvider.hpp>

namespace SpatialSEIR
{
    /*
     *
     * Implement the full conditional distribution for E_Star
     *
     */    
    
    /*TODO: make this class use E and I for steadyStateConstraintPrecision lik FC_I_Star*/
    FC_I_Star_overdispersed::FC_I_Star_overdispersed(ModelContext *_context,
                         int* _Y,
                         CompartmentalModelMatrix *_I_star,
                         CompartmentalModelMatrix *_S,  
                         CompartmentalModelMatrix *_I,  
                         CompartmentalModelMatrix *_E,
                         CompartmentalModelMatrix *_E_star,
                         CompartmentalModelMatrix *_R_star,
                         double *_p_ei,
                         double *_p_ir,
                         double *_p_se,
                         double *_phi,
                         double _steadyStateConstraintPrecision,
                         double _sliceWidth) 
    {
        context = new ModelContext*;
        I_star = new CompartmentalModelMatrix*;
        Y = new int*;
        S = new CompartmentalModelMatrix*;
        I = new CompartmentalModelMatrix*;
        E = new CompartmentalModelMatrix*;
        R_star = new CompartmentalModelMatrix*;
        E_star = new CompartmentalModelMatrix*;
        p_ei = new double*;
        p_ir = new double*;
        p_se = new double*;
        phi = new double*;
        steadyStateConstraintPrecision = new double;
        value = new long double;
        sliceWidth = new double;
        samples = new int;
        accepted = new int; 
        *samples = 0;
        *accepted = 0;
       
        *context = _context;
        *I_star = _I_star;
        *R_star = _R_star;
        *E_star = _E_star;
        *Y = _Y;
        *E = _E;
        *I = _I;
        *S = _S;
        *sliceWidth = _sliceWidth;
        *p_ir = _p_ir;
        *p_ei = _p_ei;
        *p_se = _p_se;
        *phi = _phi; 
        *steadyStateConstraintPrecision = _steadyStateConstraintPrecision;
        *value = -1.0;

        samplers = new std::vector<Sampler*>();
        currentSampler = new Sampler*;
        samplers -> push_back(new CompartmentMetropolisSampler(*context, this, (*I_star) -> data));
        samplers -> push_back(new IndexedCompartmentMetropolisSampler(*context, this, (*I_star) -> data));
        samplers -> push_back(new CompartmentMetropolisSampler_OCL(*context, this, (*I_star) -> data));
        samplers -> push_back(new CompartmentBinomialMetropolisSampler(*context, this, (*I_star) -> data, 
                                                                       (*E) -> data,
                                                                       (*I) -> data,
                                                                       *p_ei,
                                                                       *((*context) -> I_star -> nrow)));
        samplers -> push_back(new IndexedCompartmentBinomialMetropolisSampler(*context, this, (*I_star) -> data, 
                                                                       (*E) -> data,
                                                                       (*I) -> data,
                                                                       *p_ei,
                                                                       *((*context) -> I_star -> nrow)));

        samplers -> push_back(new CompartmentBinomialMixedSampler(*context, this, (*I_star) -> data, 
                                                                       (*E) -> data,
                                                                       (*I) -> data,
                                                                       *p_ei,
                                                                       *((*context) -> I_star -> nrow)));


        samplers -> push_back(new CompartmentBinomialSliceSampler(*context, this, (*I_star) -> data, 
                                                                       (*E) -> data,
                                                                       (*I) -> data,
                                                                       *p_ei,
                                                                       *((*context) -> I_star -> nrow)));

    }

    FC_I_Star_overdispersed::~FC_I_Star_overdispersed()
    {
        while((samplers -> size()) != 0){delete (*samplers).back(); (*samplers).pop_back();}

        delete samplers;
        delete I_star;
        delete E;
        delete I;
        delete R_star;
        delete E_star;
        delete S;
        delete p_se;
        delete p_ir;
        delete p_ei;
        delete phi;
        delete value;
        delete steadyStateConstraintPrecision; 
        delete context;
        delete samples;
        delete accepted;
        delete sliceWidth;
    }

    int FC_I_Star_overdispersed::evalCPU()
    {
        int i,j, compIdx;
        int nTpts = *((*I) -> nrow); 
        int nLoc = *((*I) -> ncol); 
        double p_ei_val; 
        double p_ir_val;
        double p_se_val;
        int I_val, S_val, E_val, Estar_val, Rstar_val, Istar_val, y_val;
        long double output = 0.0;
        long unsigned int R_star_sum, I_star_sum;
        int64_t aDiff; 
        double phi_val;
        phi_val = **phi;
        for (i = 0; i<nLoc; i++)
        {
            compIdx = i*nTpts;
            for (j = 0; j < nTpts; j++)
            {
                Rstar_val = ((*R_star) -> data)[compIdx];
                Estar_val = ((*E_star) -> data)[compIdx];
                E_val = ((*E) -> data)[compIdx];
                S_val = ((*S) -> data)[compIdx];
                Istar_val = ((*I_star) -> data)[compIdx];
                I_val = ((*I) -> data)[compIdx];
                y_val = (*Y)[compIdx];
                p_ei_val = (*p_ei)[j];
                p_ir_val = (*p_ir)[j]; 
                p_se_val = (*p_se)[compIdx];

                if (Istar_val < 0 || Istar_val > E_val || 
                        Rstar_val > I_val)

                {
                    *value = -INFINITY;
                    return(-1);
                }
                else
                {
                    output += (((*context) -> random -> dbinom(Estar_val, S_val, p_se_val)) +    
                               ((*context) -> random -> dbinom(Rstar_val, I_val, p_ir_val)) +    
                               ((*context) -> random -> dbinom(Istar_val, E_val, p_ei_val))); 
                    output -= 0.5*std::pow((Istar_val - y_val)*phi_val, 2);
                }
                compIdx++;
            }

            if (*steadyStateConstraintPrecision > 0)
            {
                R_star_sum = (*R_star)->marginSum(2,i);
                I_star_sum = (*I_star)->marginSum(2,i);
                aDiff = (R_star_sum > I_star_sum ? R_star_sum - I_star_sum : I_star_sum - R_star_sum)/nTpts;
                output -= (aDiff*aDiff)*(*steadyStateConstraintPrecision);
            }
        }

        if (!std::isfinite(output))
        {
            *value = -INFINITY;
            return(-1);
        }
        else
        {
            *value = output;
        }
    
       return 0;
    }

    int FC_I_Star_overdispersed::evalCPU(int startLoc, int t)
    {
        int i,j, compIdx;
        int nTpts = *((*I) -> nrow); 
        int nLoc = *((*I) -> ncol); 
        double p_ei_val; 
        double p_ir_val;
        double p_se_val;
        int S_val, I_val, E_val, Estar_val, Rstar_val, Istar_val, y_val;
        long double output = 0.0;
        long unsigned int R_star_sum, I_star_sum;
        int64_t aDiff; 
        double phi_val;
        phi_val = **phi;


        i = startLoc;
        compIdx = i*nTpts + t;
        for (j = t; j < nTpts; j++)
        {
            Rstar_val = ((*R_star) -> data)[compIdx];
            E_val = ((*E) -> data)[compIdx];
            Istar_val = ((*I_star) -> data)[compIdx];
            I_val = ((*I) -> data)[compIdx];
            y_val = (*Y)[compIdx];
            p_ei_val = (*p_ei)[j];
            p_ir_val = (*p_ir)[j]; 

            if (Istar_val < 0 || Istar_val > E_val || 
                    Rstar_val > I_val)
            {
                *value = -INFINITY;
                return(-1);
            }
            else
            {
                output += (((*context) -> random -> dbinom(Rstar_val, I_val, p_ir_val)) +    
                           ((*context) -> random -> dbinom(Istar_val, E_val, p_ei_val))); 
                output -= 0.5*std::pow((Istar_val - y_val)*phi_val, 2);
            }
            compIdx++;
        }


        for (i = 0; i<nLoc; i++)
        {
            compIdx = i*nTpts + t;
            for (j = t; j < nTpts; j++)
            {
                Estar_val = ((*E_star) -> data)[compIdx];
                S_val = ((*S) -> data)[compIdx];
                p_se_val = (*p_se)[compIdx];
                output += ((*context) -> random -> dbinom(Estar_val, S_val, p_se_val));
                compIdx++;
            }
        }


        if (*steadyStateConstraintPrecision > 0)
        {
            R_star_sum = (*R_star)->marginSum(2,i);
            I_star_sum = (*I_star)->marginSum(2,i);
            aDiff = (R_star_sum > I_star_sum ? R_star_sum - I_star_sum : I_star_sum - R_star_sum)/nTpts;
            output -= (aDiff*aDiff)*(*steadyStateConstraintPrecision);
        }


        if (!std::isfinite(output))
        {
            *value = -INFINITY;
            return(-1);
        }
        else
        {
            *value = output;
        }
    
       return 0;
    }



    int FC_I_Star_overdispersed::evalOCL()
    {
        //NOT IMPLEMENTED
        return(evalCPU());
    }
    int FC_I_Star_overdispersed::calculateRelevantCompartments()
    {
        (*context) -> calculateI_CPU();
        (*context) -> calculateE_givenI_CPU();
        (*context) -> calculateP_SE_CPU();
        return(0);
    }
    int FC_I_Star_overdispersed::calculateRelevantCompartments(int i, int j)
    {
        (*context) -> calculateI_CPU(i,j);
        (*context) -> calculateE_givenI_CPU(i,j);
        (*context) -> calculateP_SE_CPU(i,j);
        return(0);
    }

    int FC_I_Star_overdispersed::calculateRelevantCompartments_OCL()
    {
        (*context) -> calculateI_CPU();
        (*context) -> calculateE_givenI_CPU();
        (*context) -> calculateP_SE_OCL();
        return(0);
    }

    void FC_I_Star_overdispersed::sample(int verbose)
    {
        if (verbose){lssCout << "Sampling I_star\n";}
        (*context) -> cacheP_SE_Calculation();
        (*currentSampler) -> drawSample();
    }

    long double FC_I_Star_overdispersed::getValue()
    {
        return(*(this -> value));
    }
    void FC_I_Star_overdispersed::setValue(long double val)
    {
        *(this -> value) = val;
    }
}
