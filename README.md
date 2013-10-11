Perceptron
==========

cs 434 HW1: Perceptron

http://classes.engr.oregonstate.edu/eecs/fall2013/cs434/hws/hw1.pdf

We were tasked with implementing an online perceptron algorithm that read in a training file, and then learned what the weight vector would be and tested it against a test file. 

To run this:
* g++ -o perc.c
* a.out number\_of\_epochs shuffle(y/n) trainingfile testfile
* Default: a.out will call a.out with num\_epochs set to # of rows, shuffle set to false, traing file set to perceptron\_train.csv, and test\_file set to perceptron\_test.csv

