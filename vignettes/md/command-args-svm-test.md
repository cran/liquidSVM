
    
* `GPU=<gpus>`
    
    Sets the number of GPUs that are going to be used.
    Currently, there is no checking whether your system actually has <gpus> many
    GPUs. In addition, the number of used threads is reduced to <gpus>.
    
    Allowed values:
    <gpus>: integer between 0 and ???
    
    Default values:
    <gpus> = 0
    
    Unfortunately, this option is not activated for the binaries you are currently
    using. Install CUDA and recompile to activate this option.
    
    
* `h=[<level>]`
    
    Displays all help messages.
    
    Meaning of specific values:
    <level> = 0  =>  short help messages
    <level> = 1  =>  detailed help messages
    
    Allowed values:
    <level>: 0 or 1
    
    Default values:
    <level> = 0
    
    
* `L=c(<loss>,[<neg_weight>,<pos_weight>])`
    
    Sets the loss that is used to compute empirical errors. The optional weights can
    only be set, if <loss> specifies a loss that has weights.
    
    Meaning of specific values:
    <loss> = 0  =>   binary classification loss
    <loss> = 1  =>   multiclass class
    <loss> = 2  =>   least squares loss
    <loss> = 3  =>   weighted least squares loss
    <loss> = 5  =>   your own template loss
    
    Allowed values:
    <loss>: integer between 0 and 2
    <neg_weight>: float > 0.0
    <pos_weight>: float > 0.0
    
    Default values:
    <loss> = 0
    <neg_weight> = 1.0
    <pos_weight> = 1.0
    
    
* `T=<threads>`
    
    Sets the number of threads that are going to be used. Each thread is
    assigned to a logical processor on the system, so that the number of
    allowed threads is bounded by the number of logical processors. On
    systems with activated hyperthreading each physical core runs one thread,
    if <threads> does not exceed the number of physical cores. Since hyper-
    threads on the same core share resources, using more threads than cores
    does usually not increase the performance significantly, and may even
    decrease it.
    
    Meaning of specific values:
    <threads> =  0   =>   4 threads are used (all physical cores run one thread)
    <threads> = -1   =>   3 threads are used (all but one of the physical cores
                                              run one thread)
    
    Allowed values:
    <threads>: integer between -1 and 4
    
    Default values:
    <threads> = 0
    
    
* `v=c(<weighted>,<scenario>,[<npl_class>])`
    
    Sets the weighted vote method to combine decision functions from different
    folds. If <weighted> = 1, then weights are computed with the help of the
    validation error, otherwise, equal weights are used. In the classification
    scenario, the decision function values are first transformed to -1 and +1,
    before a weighted vote is performed, in the regression scenario, the bare
    function values are used in the vote. In the weighted NPL scenario, the weights
    are computed according to the validation error on the samples with label
    <npl_class>, the rest is like in the classification scenario.
    <npl_class> can only be set for the NPL scenario.
    
    Meaning of specific values:
    <scenario> = 0  =>   classification
    <scenario> = 1  =>   regression
    <scenario> = 2  =>   NPL
    
    Allowed values:
    <weighted>: 0 or 1
    <scenario>: integer between 0 and 2
    <npl_class>: -1 or 1
    
    Default values:
    <weighted> = 1
    <scenario> = 0
    <npl_class> = 1
    
    
* `o=<display_roc_style>`
    
    Sets a flag that decides, wheather classification errors are displayed by
    true positive and false positives.
    
    Allowed values:
    <display_roc_style>: 0 or 1
    
    Default values:
    <display_roc_style>: Depends on option -v
    
    
