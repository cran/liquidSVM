# Copyright 2015-2017 Philipp Thomann
#
# This file is part of liquidSVM.
#
# liquidSVM is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# liquidSVM is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with liquidSVM. If not, see <http://www.gnu.org/licenses/>.

#' liquidSVM functions for mlr
#' 
#' Allow for liquidSVM \code{\link{lsSVM}} and \code{\link{mcSVM}}
#' to be used in the \code{mlr} framework.
#' 
#' @note In order that mlr can find our learners liquidSVM has to be loaded
#' using e.g. \code{library(liquidSVM)}
#' \code{model <- train(...)}
#' @name mlr-liquidSVM
#' @param .learner see mlr-Documentation
#' @param .task see mlr-Documentation
#' @param .subset see mlr-Documentation
#' @param .weights see mlr-Documentation
#' @param .model the trained mlr-model, see mlr-Documentation
#' @param .newdata the test features, see mlr-Documentation
#' @param partition_choice the partition choice, see \link{Configuration}
#' @param partition_param a further param for partition choice, see \link{Configuration}
#' @param ... other parameters, see \link{Configuration}
#' @examples
#' \dontrun{
#' if(require(mlr)){
#' library(liquidSVM)
#' 
#' ## Define a regression task
#' task <- makeRegrTask(id = "trees", data = trees, target = "Volume")
#' ## Define the learner
#' lrn <- makeLearner("regr.liquidSVM", display=1)
#' ## Train the model use mlr::train to get the correct train function
#' model <- train(lrn,task)
#' pred <- predict(model, task=task)
#' performance(pred)
#' 
#' ## Define a classification task
#' task = makeClassifTask(id = "iris", data = iris, target = "Species")
#' 
#' ## Define the learner
#' lrn <- makeLearner("classif.liquidSVM", display=1)
#' model <- train(lrn,task)
#' pred <- predict(model, task=task)
#' performance(pred)
#' 
#' } # end if(require(mlr))
#' }
NULL


#' @export
#' @rdname mlr-liquidSVM
makeRLearner.regr.liquidSVM <- function() {
  if(!requireNamespace('mlr', quietly=TRUE)) stop("this function needs mlr to be installed")
  if(!requireNamespace('ParamHelpers', quietly=TRUE)) stop("this function needs ParamHelpers to be installed")
  mlr::makeRLearnerRegr(
    cl = "regr.liquidSVM",
    package = "liquidSVM",
    par.set = ParamHelpers::makeParamSet(
      ParamHelpers::makeLogicalLearnerParam(id = "scaled", default = TRUE),
      ParamHelpers::makeNumericLearnerParam(id = "clip", lower = -1, default = -1, ),
      ParamHelpers::makeDiscreteLearnerParam(id = "kernel", default = "GAUSS_RBF",
                 values = c("GAUSS_RBF","POISSON")),
      ParamHelpers::makeIntegerLearnerParam(id = "partition_choice", default = 0, lower = 0, upper = 6),
      ParamHelpers::makeNumericLearnerParam(id = "partition_param", default = -1,
                requires = quote(partition_choice >= 1L)),
      ParamHelpers::makeIntegerLearnerParam(id = "grid_choice", default = 0, lower = -2, upper = 2),
      ParamHelpers::makeIntegerLearnerParam(id = "folds", default = 5, lower = 1),
      ParamHelpers::makeIntegerLearnerParam(id = "display", default = 0, lower = 0, upper=7)
    ),
    #par.vals = list(fit = FALSE),
    properties = c("numerics", "factors"),
    name = "Support Vector Machines",
    short.name = "liquidSVM",
    note = "FIXME make integrated cross-validation more accessable."
  )
}

#' @export
#' @rdname mlr-liquidSVM
trainLearner.regr.liquidSVM <- function(.learner, .task, .subset, .weights = NULL, #scaled, clip, kernel,
                                       partition_choice=0, partition_param=-1, #grid_choice, folds,
                                       ...) {
  if(!requireNamespace('mlr', quietly=TRUE)) stop("this function needs mlr to be installed")
  f = mlr::getTaskFormula(.task)
  if(partition_param > 0) partition_choice <- c(partition_choice, partition_param)
  data <- mlr::getTaskData(.task, .subset)
  liquidSVM::lsSVM(f, data, partition_choice=partition_choice, ...)
}

#' @export
#' @rdname mlr-liquidSVM
predictLearner.regr.liquidSVM <- function(.learner, .model, .newdata, ...) {
  if(!requireNamespace('mlr', quietly=TRUE)) stop("this function needs mlr to be installed")
  predict.liquidSVM(.model$learner.model, newdata = .newdata, ...)#[, 1L]
}


#' @export
#' @rdname mlr-liquidSVM
makeRLearner.classif.liquidSVM <- function() {
  if(!requireNamespace('mlr', quietly=TRUE)) stop("this function needs mlr to be installed")
  if(!requireNamespace('ParamHelpers', quietly=TRUE)) stop("this function needs ParamHelpers to be installed")
  mlr::makeRLearnerClassif(
    cl = "classif.liquidSVM",
    package = "liquidSVM",
    par.set = ParamHelpers::makeParamSet(
      ParamHelpers::makeLogicalLearnerParam(id = "scaled", default = TRUE),
      ParamHelpers::makeDiscreteLearnerParam(id = "mc_type", default = "mc_AvA", values = c("mc_AvA","mc_OvA","mc_OvA_hi")),
      ParamHelpers::makeDiscreteLearnerParam(id = "kernel", default = "GAUSS_RBF",
                               values = c("GAUSS_RBF","POISSON")),
      ParamHelpers::makeIntegerLearnerParam(id = "partition_choice", default = 0, lower = 0, upper = 6),
      ParamHelpers::makeNumericLearnerParam(id = "partition_param", default = -1,
                              requires = quote(partition_choice >= 1L)),
      ParamHelpers::makeIntegerLearnerParam(id = "grid_choice", default = 0, lower = -2, upper = 2),
      ParamHelpers::makeIntegerLearnerParam(id = "folds", default = 5, lower = 1),
      ParamHelpers::makeIntegerLearnerParam(id = "display", default = 0, lower = 0, upper=7),
      ParamHelpers::makeNumericVectorLearnerParam(id = "weights", len = NA_integer_, lower = 0)
    ),
    #par.vals = list(fit = FALSE),
    properties = c("twoclass", "multiclass", "numerics", "factors", "prob", "class.weights"),
    class.weights.param = "weights",
    name = "Support Vector Machines",
    short.name = "liquidSVM",
    note = "FIXME make integrated cross-validation more accessable."
  )
}

#' @export
#' @rdname mlr-liquidSVM
trainLearner.classif.liquidSVM <- function(.learner, .task, .subset, .weights = NULL, #scaled, clip, kernel,
                                            partition_choice=0, partition_param=-1, #grid_choice, folds,
                                            ...) {
  if(!requireNamespace('mlr', quietly=TRUE)) stop("this function needs mlr to be installed")
  if(partition_param > 0) partition_choice <- c(partition_choice, partition_param)
  f <-  mlr::getTaskFormula(.task)
  data <- mlr::getTaskData(.task, .subset)
  liquidSVM::mcSVM(f, data, partition_choice=partition_choice, ...)
}

#' @export
#' @rdname mlr-liquidSVM
predictLearner.classif.liquidSVM <- function(.learner, .model, .newdata, ...) {
  if(!requireNamespace('mlr', quietly=TRUE)) stop("this function needs mlr to be installed")
  predict.liquidSVM(.model$learner.model, newdata = .newdata, ...)
}


