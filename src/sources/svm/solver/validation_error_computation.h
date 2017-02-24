// Copyright 2015, 2016, 2017 Ingo Steinwart
//
// This file is part of liquidSVM.
//
// liquidSVM is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as 
// published by the Free Software Foundation, either version 3 of the 
// License, or (at your option) any later version.
//
// liquidSVM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with liquidSVM. If not, see <http://www.gnu.org/licenses/>.


#if !defined (VALIDATION_ERROR_COMPUTATION_H)
	#define VALIDATION_ERROR_COMPUTATION_H


//**********************************************************************************************************************************


__global__ void evaluate_val_predictions(double* validation_kernel_GPU, double* prediction_GPU, unsigned* coefficient_changed_GPU, double* coefficient_delta_GPU, unsigned col_set_size, unsigned col_set_size_aligned, unsigned number_coefficients_changed); 


//**********************************************************************************************************************************


#if !defined(COMPILE_SEPERATELY__) && !defined(COMPILE_SEPERATELY__CUDA)
	#include "sources/svm/solver/validation_error_computation.cu"
#endif

#endif

