#include "stdio.h"
#include "vc.h"

//Criar uma imagem bin�ria e gravar para ficheiro
int main_vc0003(void)
{
	IVC *image;
	int x, y;
	long int pos;

	image = vc_image_new(320, 280, 1, 1);
	if(image == NULL){
		printf("ERROR -> vc_image_new():\n\tOut of memory!\n");
		getchar();
		return 0;
	}

	for (x = 0; x < image->width; x++) {
		for (y = 0; y < image->height; y++) {	
			pos = y * image->bytesperline + x * image->channels;

			if ((x <= image->width / 2) && (y <= image->height / 2)) image->data[pos] = 1;
			else if ((x > image->width / 2) && (y > image->height / 2)) image->data[pos] = 1;
			else image->data[pos] = 0;
			}
		}

	vc_write_image("teste.pbm", image);

	vc_image_free(image);

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}
