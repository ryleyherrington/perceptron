#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//#define DEBUG 0


class Xvec {
  public:
    float dotprod(std::vector<float> w);
    Xvec() { xd[0] = xd[1] = xd[2] = xd[3] = 0.0; }
  public:
    float xd[4];
};

typedef std::vector<Xvec> XDATA;
typedef std::vector<int> YDATA;

void shuffle(XDATA &x, YDATA &y, int epochs)
{
    for (int i=0; i<epochs; i++) {
        int n = random() % x.size();
        if (n == i) continue;
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

float Xvec::dotprod(std::vector<float> w)
{
	float retval = 0;	
	for (int i =0; i<w.size(); i++) {
		retval += ( xd[i] * w[i]);
	}
	return retval;
}

bool update_weights(Xvec x, int y, std::vector<float> &w) 
{
    if ((x.dotprod(w) * y) <= 0) {
        for (int j=0; j<w.size(); j++) {
            w[j] = y * x.xd[j] + w[j];
        }
        return true;
    }
    return false;
}

std::vector<float> p_train(XDATA x, YDATA y, int epochs, bool shuffled_flag)
{
    std::vector<float> w;
    w.resize(4);

    bool updated = true;
    while (updated) {
        updated = false;
        for (int i=0; i<epochs; i++) {
			updated = update_weights(x[i], y[i], w);
    		if (shuffled_flag) 
				shuffle(x, y, epochs);
        }
    }
    return w;
}

int p_classify(Xvec x, std::vector<float> w) 
{
    return x.dotprod(w) > 0 ? 1 : -1;
}

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

#ifdef DEBUG
    for (int i=0; i<x.size(); i++) {
        printf("%d, %f, %f, %f, %f\n", 
               y[i], x[i].xd[0], x[i].xd[1], x[i].xd[2], x[i].xd[3]);
    }
#endif
}

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
    bool do_shuffle  = argc>2 ? *argv[2] == 'y' : true;
    FILE *train_file = openfile( argc>3 ? argv[3] : (char*)"perceptron_train.csv");
    FILE *test_file  = openfile( argc>4 ? argv[4] : (char*)"perceptron_test.csv");

    XDATA xtrain, xtest;
    YDATA ytrain, ytest;
    read_data(train_file, xtrain, ytrain);
    //fprintf(stdout, "==============================================\n");

    if (num_epochs == 0)
        num_epochs = xtrain.size();
    std::vector<float> w = p_train(xtrain, ytrain, num_epochs, do_shuffle);
    fprintf(stdout, "weight vector = %f %f %f %f\n", w[0],w[1],w[2],w[3]);

    //fprintf(stdout, "==============================================\n");
    int failcount = 0;
    read_data(test_file,  xtest,  ytest);
    for (int i=0; i<xtest.size(); i++){
        int y = p_classify(xtest[i], w);
        if (y != ytest[i]) {
            update_weights(xtest[i], ytest[i], w);
            y = p_classify(xtest[i], w);
            if (y != ytest[i]) {
                failcount++;
            }
        }
#if 0
        if (y != ytest[i]) {
            fprintf(stdout, "Bad classification on: %f %f %f\n",
                xtest[i].xd[1], xtest[i].xd[2], xtest[i].xd[3]);
            fprintf(stdout, "Correct y = %d, calculated y = %d\n", ytest[i], y);
        }
        else {
            fprintf(stdout, "Got one right\n");
        }
#endif
    }

    //fprintf(stdout, "==============================================\n");
    fprintf(stdout, "Failures = %d\nEpochs = %d\n\n", failcount, num_epochs);
    return 0;
}

