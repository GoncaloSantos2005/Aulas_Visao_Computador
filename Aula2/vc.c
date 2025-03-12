//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2022/2023
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    VISÃO POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita (no MSVC++) warnings de funções não seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "vc.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUNÇÕES: ALOCAR E LIBERTAR UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Alocar memória para uma imagem
IVC* vc_image_new(int width, int height, int channels, int levels)
{
	IVC* image = (IVC*)malloc(sizeof(IVC));

	if (image == NULL) return NULL;
	if ((levels <= 0) || (levels > 255)) return NULL;

	image->width = width;
	image->height = height;
	image->channels = channels;
	image->levels = levels;
	image->bytesperline = image->width * image->channels;
	image->data = (unsigned char*)malloc(image->width * image->height * image->channels * sizeof(char));

	if (image->data == NULL)
	{
		return vc_image_free(image);
	}

	return image;
}


// Libertar memória de uma imagem
IVC* vc_image_free(IVC* image)
{
	if (image != NULL)
	{
		if (image->data != NULL)
		{
			free(image->data);
			image->data = NULL;
		}

		free(image);
		image = NULL;
	}

	return image;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    FUNÇÕES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


char* netpbm_get_token(FILE* file, char* tok, int len)
{
	char* t;
	int c;

	for (;;)
	{
		while (isspace(c = getc(file)));
		if (c != '#') break;
		do c = getc(file);
		while ((c != '\n') && (c != EOF));
		if (c == EOF) break;
	}

	t = tok;

	if (c != EOF)
	{
		do
		{
			*t++ = c;
			c = getc(file);
		} while ((!isspace(c)) && (c != '#') && (c != EOF) && (t - tok < len - 1));

		if (c == '#') ungetc(c, file);
	}

	*t = 0;

	return tok;
}


long int unsigned_char_to_bit(unsigned char* datauchar, unsigned char* databit, int width, int height)
{
	int x, y;
	int countbits;
	long int pos, counttotalbytes;
	unsigned char* p = databit;

	*p = 0;
	countbits = 1;
	counttotalbytes = 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//*p |= (datauchar[pos] != 0) << (8 - countbits);

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				*p |= (datauchar[pos] == 0) << (8 - countbits);

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				*p = 0;
				countbits = 1;
				counttotalbytes++;
			}
		}
	}

	return counttotalbytes;
}


void bit_to_unsigned_char(unsigned char* databit, unsigned char* datauchar, int width, int height)
{
	int x, y;
	int countbits;
	long int pos;
	unsigned char* p = databit;

	countbits = 1;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//datauchar[pos] = (*p & (1 << (8 - countbits))) ? 1 : 0;

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				datauchar[pos] = (*p & (1 << (8 - countbits))) ? 0 : 1;

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				countbits = 1;
			}
		}
	}
}


IVC* vc_read_image(char* filename)
{
	FILE* file = NULL;
	IVC* image = NULL;
	unsigned char* tmp;
	char tok[20];
	long int size, sizeofbinarydata;
	int width, height, channels;
	int levels = 255;
	int v;

	// Abre o ficheiro
	if ((file = fopen(filename, "rb")) != NULL)
	{
		// Efectua a leitura do header
		netpbm_get_token(file, tok, sizeof(tok));

		if (strcmp(tok, "P4") == 0) { channels = 1; levels = 1; }	// Se PBM (Binary [0,1])
		else if (strcmp(tok, "P5") == 0) channels = 1;				// Se PGM (Gray [0,MAX(level,255)])
		else if (strcmp(tok, "P6") == 0) channels = 3;				// Se PPM (RGB [0,MAX(level,255)])
		else
		{
#ifdef VC_DEBUG
			printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM, PGM or PPM file.\n\tBad magic number!\n");
#endif

			fclose(file);
			return NULL;
		}

		if (levels == 1) // PBM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca memória para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL) return NULL;

			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height;
			tmp = (unsigned char*)malloc(sizeofbinarydata);
			if (tmp == NULL) return 0;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			if ((v = fread(tmp, sizeof(unsigned char), sizeofbinarydata, file)) != sizeofbinarydata)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				free(tmp);
				return NULL;
			}

			bit_to_unsigned_char(tmp, image->data, image->width, image->height);

			free(tmp);
		}
		else // PGM ou PPM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &levels) != 1 || levels <= 0 || levels > 255)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PGM or PPM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca memória para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL) return NULL;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			size = image->width * image->height * image->channels;

			if ((v = fread(image->data, sizeof(unsigned char), size, file)) != size)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				return NULL;
			}
		}

		fclose(file);
	}
	else
	{
#ifdef VC_DEBUG
		printf("ERROR -> vc_read_image():\n\tFile not found.\n");
#endif
	}

	return image;
}


int vc_write_image(char* filename, IVC* image)
{
	FILE* file = NULL;
	unsigned char* tmp;
	long int totalbytes, sizeofbinarydata;

	if (image == NULL) return 0;

	if ((file = fopen(filename, "wb")) != NULL)
	{
		if (image->levels == 1)
		{
			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height + 1;
			tmp = (unsigned char*)malloc(sizeofbinarydata);
			if (tmp == NULL) return 0;

			fprintf(file, "%s %d %d\n", "P4", image->width, image->height);

			totalbytes = unsigned_char_to_bit(image->data, tmp, image->width, image->height);
			printf("Total = %ld\n", totalbytes);
			if (fwrite(tmp, sizeof(unsigned char), totalbytes, file) != totalbytes)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				free(tmp);
				return 0;
			}

			free(tmp);
		}
		else
		{
			fprintf(file, "%s %d %d 255\n", (image->channels == 1) ? "P5" : "P6", image->width, image->height);

			if (fwrite(image->data, image->bytesperline, image->height, file) != image->height)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				return 0;
			}
		}

		fclose(file);

		return 1;
	}

	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    FUNÇÕES: ESPAÇOS DE COR
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Função que calcula o negativo de uma imagem em tons de cinzento
int vc_gray_negative(IVC *srcdst)
{ 
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytheperline = srcdst->bytesperline;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)){return 0;}
	if (channels != 1) { return 0; }

	//Inverter a imagem Gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytheperline + x * channels;
			//data[pos] = 255 - data[pos];
			data[pos] = srcdst->levels - data[pos];
		}
	}
	return 1;
}

//Gerar negativo da imagem RGB
int vc_rgb_negative(IVC* srcdst) 
{
	unsigned char* data = (unsigned char*)srcdst->data;	
	int width = srcdst->width;
	int height = srcdst->height;	
	int bytheperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) { return 0; }
	if (channels != 3) return 0;

	//Inverter a imagem RGB
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos = y * bytheperline + x * channels;
			data[pos] = 255 - data[pos];
			data[pos + 1] = 255 - data[pos + 1];
			data[pos + 2] = 255 - data[pos + 2];
		}
	}
	return 1;
}

int vc_rgb_to_gray(IVC* src, IVC* dst)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf;

	//Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 3) || (dst->channels != 1)) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;
				
			rf = (float)datasrc[pos_src];
			gf = (float)datasrc[pos_src + 1];
			bf = (float)datasrc[pos_src + 2];

			datadst[pos_dst] = (unsigned char)((rf * 0.299) + (gf * 0.587) + (bf * 0.114));
		}
	}
	return 1;
}

int vc_rgb_to_hsv(IVC* src, IVC* dst)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf, hue, max, min;

	//Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 3) || (dst->channels != 3)) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			rf = (float)datasrc[pos_src];
			gf = (float)datasrc[pos_src + 1];
			bf = (float)datasrc[pos_src + 2];

			//H: dst->data[pos]
			//S: dst->data[pos+1]
			//V: dst->data[pos+2

			//-> max:
			max = MAX3(rf,gf,bf);
			//-> min:
			min = MIN3(rf, gf, bf);

			if (max == rf) {
				if (gf >= bf) {
					hue = 60 * ((gf - bf) / (max - min));
				}
				else if(bf > gf) {
					hue = 360 + 60 * ((gf - bf) / (max - min));
				}
			}
			else if (max == gf) {
				hue = 120 + 60 * ((bf - rf) / (max - min));
			}
			else if (max == bf) {
				hue = 240 + 60 * ((rf - gf) / (max - min));
			}

			dst->data[pos_dst] = (hue / 360) * 255;
			if (max != min)
				dst->data[pos_dst + 1] = (max - min) / max * 255;
			else
				dst->data[pos_dst + 1] = 0;
			dst->data[pos_dst + 2] = max;

		}
	}
	return 1;
}

int vc_hsv_segmentation(IVC* src, IVC* dst, int hmin, int hmax, int smin, int smax, int vmin, int vmax)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float hs, ss, vs;
	float contador=0;

	//Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 3) || (dst->channels != 1)) return 0;
	if (dst->levels != 255) return 0;
	if (!(hmin >= 0 && hmax <= 360)) return 0;
	if (!(smin >= 0 && smax <= 255)) return 0;
	if (!(vmin >= 0 && vmax <= 255)) return 0;

	hmin = ((float)hmin * 255)/360;
	hmax = ((float)hmax * 255)/ 360;
	smin = ((float)smin * 255)/ 100;
	smax = ((float)smax * 255)/ 100;
	vmin = ((float)vmin * 255)/ 100;
	vmax = ((float)vmax * 255)/ 100;

	//h/360*255
	//s/100*255
	//v/100*255

	for (y = 0; y < height; y++){
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;
			
			hs = datasrc[pos_src];
			ss = datasrc[pos_src + 1];
			vs = datasrc[pos_src + 2];

			/*if(hs >= hmin && hs <= hmax)
				datadst[pos_dst] = 255;
			else
				datadst[pos_dst] = 0;
			if(ss >= smin && ss <= smax)
				datadst[pos_dst + 1] = 255;
			else
				datadst[pos_dst + 1] = 0;
			if (vs >= vmin && vs <= vmax)
				datadst[pos_dst + 2] = 255;
			else
				datadst[pos_dst + 2] = 0;*/
			if (hs >= hmin && hs <= hmax && ss >= smin && ss <= smax && vs >= vmin && vs <= vmax)
			{
				datadst[pos_dst] = 255;
				contador++;
			}
			else {
				datadst[pos_dst] = 0;
			}
		}
	}
	return contador;
}

int vc_scale_gray_to_color_palette(IVC* src, IVC* dst) {
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf;

	//Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 1) || (dst->channels != 3)) return 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			if (datasrc[pos_src] <= 63)
			{
				rf = 0;
				gf = (float)datasrc[pos_src] * 4;
				bf = 255;
			}
			else if (datasrc[pos_src] >= 64 && datasrc[pos_src] <= 127)
			{
				rf = 0;
				gf = 255;
				bf = 255 - ((float)datasrc[pos_src] - 64) * 4;
			}
			else if (datasrc[pos_src] >= 128 && datasrc[pos_src] <= 191)
			{
				rf = ((float)datasrc[pos_src] - 128) * 4;
				gf = 255;
				bf = 0;
			}
			else
			{
				rf = 255;
				gf = 255 - ((float)datasrc[pos_src] - 192) * 4;
				bf = 0;
			}

			datadst[pos_dst] = rf;
			datadst[pos_dst + 1] = gf;
			datadst[pos_dst + 2] = bf;
		}
	}
	return 1;
}

int vce_brain_percentage(IVC* src) {
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src;
	float rf, gf, bf;
	float rp = 0, yp = 0, gp = 0, bp = 0, brp = 0;

	//Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->channels != 3)) return 0;

	IVC *imageDST = vc_image_new(src->width, src->height, 1, src->levels);

	if (imageDST == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();	
		return 0;
	}

	//red
	rp = rp + vc_hsv_segmentation(src, imageDST, 0, 45, 50, 100, 50, 100);
	vc_write_image("vc0008r1.ppm", imageDST);
	rp = rp + vc_hsv_segmentation(src, imageDST, 291, 360, 50, 100, 50, 100);
	vc_write_image("vc0008r2.ppm", imageDST);

	//yellow
	yp = yp + vc_hsv_segmentation(src, imageDST, 46, 70, 50, 100, 50, 100);
	vc_write_image("vc0008y.ppm", imageDST);

	//green
	gp = gp + vc_hsv_segmentation(src, imageDST, 71, 160, 50, 100, 14, 100); 
	vc_write_image("vc0008g.ppm", imageDST);

	//blue 
	bp = bp + vc_hsv_segmentation(src, imageDST, 161, 290, 50, 100, 14, 100);
	vc_write_image("vc0008b.ppm", imageDST);

	//brain
	brp = brp + vc_hsv_segmentation(src, imageDST, 1, 359, 50, 100, 10, 100);
	vc_write_image("vc0008w.ppm", imageDST);

	printf("----- Total pixels -----\n");
	printf("Red: %.0f\n", rp);
	printf("Yellow: %.0f\n", yp);
	printf("Green: %.0f\n", gp);
	printf("Blue: %.0f\n", bp);
	printf("Brain: %.0f\n", brp);

	printf("----- Percentage -----\n");
    printf("- %.2f %% of brain with 76%% at 100%% brain activity\n", (rp / brp) * 100);
    printf("- %.2f %% of brain with 51%% at 75%% brain activity\n", (yp / brp) * 100);
    printf("- %.2f %% of brain with 26%% at 50%% brain activity\n", (gp / brp) * 100);
    printf("- %.2f %% of brain with 0%% at 25%% brain activity\n\n\n", (bp / brp) * 100);
	vc_image_free(imageDST);

	return 1;
}