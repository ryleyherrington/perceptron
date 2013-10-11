#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//This will hold our Xvectors[x1-x3]
class Xvec {
  public:
    float dotprod(std::vector<float> w);
    Xvec() { xd[0] = xd[1] = xd[2] = xd[3] = 0.0; }
  public:
	//include our hardcoded "1.000"
    float xd[4];
};

typedef std::vector<Xvec> XDATA; //Will return our x vals
typedef std::vector<int> YDATA;  //Will return our y val

//Shuffle all of the rows given our vector of inputs
void shuffle(XDATA &x, YDATA &y, int epochs)
{
    for (int i=0; i<epochs; i++) {
        int n = random() % x.size();
        if (n == i) continue; //Obviously if n==i don't move it. Wasted computation.
        // shuffle x[i]
        Xvec tx = x[i];
        x[i] = x[n];
        x[n] = tx;
        // shuffle y[i]
        int ty = y[i];
        y[i] = y[n];
        y[n] = ty;
    }
}

//Dot product helper function
float Xvec::dotprod(std::vector<float> w)
{
	float retval = 0;	
	for (int i =0; i<w.size(); i++) {
		retval += ( xd[i] * w[i]);
	}
	return retval;
}
int test_fails;
//abstracted out this to help when we read in a failure guess
//and instead of redoing it from the beginning, we update w
bool update_weights(Xvec x, int y, std::vector<float> &w) 
{
    if ((x.dotprod(w) * y) <= 0) {
		test_fails++;
        for (int j=0; j<w.size(); j++) {
            w[j] = y * x.xd[j] + w[j];
        }
        return true;
    }
    return false;
}

//p_train based on the online perceptron algorithm 
std::vector<float> p_train(XDATA x, YDATA y, int epochs, bool shuffled_flag)
{
    std::vector<float> w;
    w.resize(4); //make w a vector of size 4 initialized to 0

    bool updated = true;
    while (updated) { //while we are reading through
        updated = false;
        for (int i=0; i<epochs; i++) { //For how many iterations:
			updated = update_weights(x[i], y[i], w); //update the weight each row
    		if (shuffled_flag) 
				shuffle(x, y, epochs);//shuffle if we want to
        }
    }
    return w;
}

//Classifier: takes a row(with x and y) and a weight 
//and returns if it's classified correctly
int p_classify(Xvec x, std::vector<float> w) 
{
    return x.dotprod(w) > 0 ? 1 : -1;
}

//Read the data (whether test or training) and modifies x and y 
void read_data(FILE *f, XDATA &x, YDATA &y)
{
    char inbuff[200];
    while (fgets(inbuff, sizeof(inbuff), f) != NULL) {

        int yval;
        Xvec xval;
        sscanf(inbuff, "%d,%f,%f,%f", &yval, &xval.xd[1], &xval.xd[2], &xval.xd[3]);
        xval.xd[0] = 1.0;

        y.push_back(yval);
        x.push_back(xval);
    }
}

//self explantory... it opens a file.
FILE* openfile(char* fname)
{
    FILE *f;
    if (!fname) {
        fprintf(stdout, "Usage: a.out training_data test_data\n");
        exit(0);
    }
    if ( (f = fopen(fname, "r")) == NULL) {
        fprintf(stdout, "Can't open file '%s'\n", fname);
        perror("fopen");
        exit(1);
    }
    return f;
}


int main(int argc, char *argv[])
{
    int  num_epochs  = argc>1 ? atoi(argv[1]) : 0;
    bool do_shuffle  = argc>2 ? *argv[2] == 'y' : false;
    FILE *train_file = openfile( argc>3 ? argv[3] : (char*)"perceptron_train.csv");
    FILE *test_file  = openfile( argc>4 ? argv[4] : (char*)"perceptron_test.csv");

    XDATA xtrain, xtest;
    YDATA ytrain, ytest;
    read_data(train_file, xtrain, ytrain);

    if (num_epochs == 0)
        num_epochs = xtrain.size();
	//read in training data and figure out w based on those
    std::vector<float> w = p_train(xtrain, ytrain, num_epochs, do_shuffle);
    fprintf(stdout, "weight vector = %f %f %f %f\n", w[0],w[1],w[2],w[3]);

    int failcount = 0;
	//read in test data
    read_data(test_file,  xtest,  ytest);
    for (int i=0; i<xtest.size(); i++){
		//call classify on test_file and until it fails just keep going
        int y = p_classify(xtest[i], w);
		//if we fail, it means we need to re classify
        if (y != ytest[i]) { 
			//update the weights since we failed
            update_weights(xtest[i], ytest[i], w);
			//change it so that we learned
            y = p_classify(xtest[i], w);
            if (y != ytest[i]) {
				//update failure count so we know that we got better
                failcount++;
            }
        }
    }

    fprintf(stdout, "Failures = %d\nEpochs = %d\n\n", failcount, num_epochs);
    fprintf(stdout, "Failures in training: %d\n", test_fails);
    return 0;
}

