# Fisher's Linear Discriminant

Implementation of Fisher's Linear Discriminant for binary classification as described in Section 4.1.4 Bishop, C. (2006). Pattern Recognition and Machine Learning.

### Data Set

* Training Data (960 instances) 
* Testing data (412 instances)
* The dataset was taken from uci repository’s ’banknote authentication Data Set’. Each data instance has five values:
  * variance of Wavelet Transformed image (continuous)
  * skewness of Wavelet Transformed image (continuous)
  * curtosis of Wavelet Transformed image (continuous)
  * entropy of image (continuous)
  * class (0 or 1).
  
  Four continuous values and a binary class.
  
### Results

`Accuracy` : 0.987864 | `Precision`: 1.000000 | `Recall`   :    0.972973  

| Confusion Matrix | *Predicted = 0* | *Predicted = 1*|
|  --------------  |  -------------  |  ------------- |
|   *Actual = 0*   | 	     227       |		    0	      |
|   *Actual = 1*   | 	      5	       |   	   180		  |
