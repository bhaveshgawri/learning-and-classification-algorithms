# Neural Network - Handwriting Recognition

### Objective
Recognition of handwritten digits from 0-9 using Neural Networks.

### Network Architecture and features
* 3 layers
  * Layer 1[ Input Layer]: 64   units
  * Layer 2[Hidden Layer]: 5-10 units
  * Layer 3[Output Layer]: 10   units
*  Minibatch gradient descent with momentum and adaptive learning rate with batchsize=100.
  * Implemented [Adam](https://arxiv.org/pdf/1412.6980.pdf) optimizer
  * Parameters: `alpha`: 1e-3 | `beta1`: 0.9 | `beta2`: 0.999 | `epsilon`: 1e-8
* Validation set used to control number of iterations, i.e training stopped when error on validation set increases. 
  * Maximum number of iterations set at 3000.
* Softmax function used for the error calculation.
* Randomly initialized weights [-1, 1].

### Dataset
> UCI repository’s ”Optical Recognition of HandwrittenDigits”. 

* Each row contains 65 values. 
  * First 64 are input attributes in range [0,1] 
  * the last value is class code from 0 to 9
* The dataset is split into 3
  * Training set (3100 instances)
  * Testing set  (1670 instances)
  * Validation set (850 instances)
  
### Results
Hidden units were varied from 5-10 and following results were obtained

|HiddenL Size|Validation Thresh.|Iterations| Train Accr.|Validation Accr.|Test Accr.|	
|------------|-----------------|----------|------------|-----------------|----------|
|5	|0.92|   3000| 	 	0.93|  0.915294|0.909581|
|6|  0.92|   2128|	 	0.941613|  0.921176|0.917964|
|7	|0.92|   1368|	 	0.920323|  0.921176|0.900599|
|8|  0.94|   1716|	0.956452|  0.941176|0.931138|
|9	|0.94|   1806|	0.949032|  0.941176|0.935329|
|10| 0.94|   1727|	0.951935|  0.941176|0.933533|
