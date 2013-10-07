#include <stdio.h>
#include <stdlib.h>
#include <vector>

struct Data {
   int valid;
   float x,y,z;
};

std::vector<float> p_train(std::vector<Data> data, int epochs) 
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
