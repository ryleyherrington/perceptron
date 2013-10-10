#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

class Data {
  public:
  int y;
  float x1, x2, x3;


   int valid;
   float x,y,z;
};

void shuffle(std:vector<Data>, int n)
{
    // shuffle the first "n" elements of the vector
    return;
}

float dotprod(float x, std::vector<float> w)
{
	float retval = 0;	
	for (int i =0; i<w.size(), i++) {
		retval += (x * w[i]);
	}
	return retval;
}

std::vector<float> p_train(std::vector<Data> data, int epochs, bool shuffled=0)
{
    std::vector<float> w;  //initializes to all 0's

    bool updated = true;
    while (updated) {
        if (shuffled) 
            shuffle(data, epochs);
        updated = false;
        for (int i=0; i<epochs; i++) {
            if ((dotprod(data[i].x, w) * data[i].y) <= 0) {
                for (int j=0; j<w.size(); j++) {
                    w[j] = data[i].y * data[j].x + w[j];
                }
                updated = true;
            }
        }
    }
    return w;
}

/* Python version
 * def p_train(xs,  ys, shuffled=False, epoch=0):
 * 		w=list(repeat(0, len(xs[0]))
 * 		dotprod=lambda x,w:sum(imap(mul, x, w)
 * 		updated =True
 *		data = zip(xs, ys)
 *		if epoch:
			//pack input vectors with outputs
 *			data= data[:epoch]
 *			while updated:
 *				if shuffled:
 *					random.shuffle(data)
 *				updated = False
 *				for x,y in data:
					//check if correctly classified
 *					if dotprod(x,w) * y <=0:
 *						w = [y* xi + wi fpr xi wi in izip(x,w)]
 *						updated=True
			//return a function creates the learned decision boundary
 *			return lambda x: -(w[0] + x * w[1])/w[2]
			//y = mx + b
			//y_guess_line = -(w[1]x + w[0]) / w[2] 
 */


float p_classify(float x, std::vector<float> w) 
{
	float y;


	return y;
}

int main(int argc, char *argv[])
{
    FILE *input_file;

    if (argc > 1)
        input_file = fopen(argv[1], "r");

    if (input_file == NULL) {
        fprintf(stderr, "Can't open input file.\n");
        exit(1);
    }
    char inbuff[200];
    std::vector<Data> data;
    while (fgets(inbuff, sizeof(inbuff), input_file) != NULL) {
        Data d;
        sscanf(inbuff, "%d,%f,%f,%f", &d.valid, &d.x, &d.y, &d.z);
        data.push_back(d);
    }
    for (int i=0; i<data.size(); i++) {
        printf("%d, %f, %f, %f\n", data[i].valid, data[i].x, data[i].y, data[i].z);
    }

    return 0;
}
