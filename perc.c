#include <stdio.h>
#include <stdlib.h>
#include <vector>

struct Data {
   int valid;
   float x,y,z;
};

int dotprod(std::vector<float> x, std::vector<float> w)
{
	dotprod = 0;	
	for (int i =0; i<x.size(), i++){
		dotprod+= (x[i] * w[i]);		
	}
}

std::vector<float> p_train(std::vector<Data> data, int epochs, bool shuffled=0) 
{
    std::vector<float> w;//initializes to all 0's

    std::vector<float> u;
    u.resize(data.size());
    for (int i=0; i<data.size(); i++) {
        for (int ep=0; ep<epochs; ep++) {
            u[i] = data[i].x * w[i];
            if (data[i].y * u[i] <= 0) {
                w = w + data[i].y * data[i].x;
            }
        }
    }


    return w;
}

/* Python version
 * def p_train(xs,  ys, shuffled=False, epoch=0):
 * 		w=list(repeat(0, len(xs[0]))
 * 		dotprod=lambda x,w: 
 * 			sum(imap(mul, x, w)
 * 			updated =True
 *		data = zip(xs, ys)
 *		if epoch:
 *			data= data[:epoch]
 *			while updated:
 *				if shuffled:
 *					random.shuffle(data)
 *				updated = False
 *				for x,y in data:
 *					if dotprod(x,w) * y <=0:
 *						w = [y* xi + wi fpr xi wi in izip(x,w)]
 *						updated=True
 *			return lambda x: -(w[0] + x * w[1]/w[2]
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
