#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<stdlib.h>
#include<fitsio.h>
#include<string.h>

#include "var.h"
#include "function.h"

//Extracting information from FITS files
void read_fits(char *fits_file,NCstar *nstar)
{
	fitsfile *fptr;
	int i, j, k, col_tp, kk;
	
	unsigned char  bnul;
    	short          inul;
    	int            knul;
    	long           jnul;
    	float          enul;
    	double         dnul;
    	int  colnum, typecode;
    	char colname[70];
    	int status = 0;
	long repeat, width;
	int anynull;
    
	char fits_file_part[1000];
	
	strcpy(fits_file_part, fits_file);
	strcat(fits_file_part, "+2");	
	
	//Open the FITS file
	fits_open_file(&fptr, fits_file_part, READONLY, &status);
	
	//Get the number of rows
    	fits_get_num_rows(fptr, &nrows, &status);
	
	//Get the number of columns
	fits_get_num_cols(fptr, &ncols, &status);
	
	col_tp = col_need_num;
	col_need_num = ncols;
	
	nstar -> qstar = (QCstar *)malloc(nrows*sizeof(QCstar));
	
	//Initialization
	type_num.double_num = 0;
	type_num.float_num = 0;
	type_num.byte_num = 0;
	type_num.int_num = 0;
	type_num.long_num = 0;
	type_num.short_num = 0;

	//follow the configuration to record the colnum
	for(i = 0; i < col_tp; i++)
	{
		fits_get_colname(fptr, CASESEN, col_name[i], colname, &colnum, &status);
		//status=0;
		//printf("%d\n",status);
		//printf("%s\n",col_name[i]);
		if(status == 0)
		{
			col_exist[i] = 1;
			fits_get_coltype(fptr, colnum, &typecode, &repeat, &width, &status);
			
			col_type[i] = typecode;
			
			if(typecode == TFLOAT)
			{
				//nstar.float_data[type_num.float_num]=(float *)malloc(nrows*sizeof(float));
				//fits_read_col(fptr, typecode,i+1, 1, 1, nrows, &dnul, nstar.float_data[type_num.float_num], &anynull, &status);
				type_num.float_num++;
			}
			else if(typecode == TDOUBLE)
			{
				//nstar.double_data[type_num.double_num]=(double *)malloc(nrows*sizeof(double));
				//fits_read_col(fptr, typecode,i+1, 1, 1, nrows, &dnul, nstar.double_data[type_num.double_num], &anynull, &status);
				type_num.double_num++;
			}
			else if(typecode == TSHORT)
			{
				//nstar.short_data[type_num.short_num]=(short *)malloc(nrows*sizeof(short));
				//fits_read_col(fptr, typecode,i+1, 1, 1, nrows, &dnul, nstar.short_data[type_num.short_num], &anynull, &status);
				type_num.short_num++;
			}
			else if(typecode == TLONG)
			{
				//nstar.long_data[type_num.long_num]=(float *)malloc(nrows*sizeof(long));
				//fits_read_col(fptr, typecode,i+1, 1, 1, nrows, &dnul, nstar.long_data[type_num.long_num], &anynull, &status);
				type_num.long_num++;
			}
			else if(typecode == TINT)
			{
				//nstar.int_data[type_num.int_num]=(int *)malloc(nrows*sizeof(int));
				//fits_read_col(fptr, typecode,i+1, 1, 1, nrows, &dnul, nstar.int_data[type_num.int_num], &anynull, &status);
				type_num.int_num++;
			}
			else if(typecode == TBYTE)
			{
				//nstar.byte_data[type_num.byte_num]=(unsigned char *)malloc(nrows*sizeof(unsigned char));
				//fits_read_col(fptr, typecode,i+1, 1, 1, nrows, &dnul, nstar.byte_data[type_num.byte_num], &anynull, &status);
				type_num.byte_num++;
			}
		}
		else
		{
			//printf("%s\n",col_name[i]);
			col_exist[i] = 0;
		}
	}
	
	//printf("float is %d\n",type_num.float_num);
	//printf("double is %d\n",type_num.double_num);

	//Allocate space
	nstar->float_data = (float **)malloc(type_num.float_num * sizeof(float *));
	nstar->double_data = (double **)malloc(type_num.double_num * sizeof(double *));
	nstar->short_data = (short **)malloc(type_num.short_num * sizeof(short *));
	nstar->int_data = (int **)malloc(type_num.int_num * sizeof(int *));
	nstar->long_data = (long **)malloc(type_num.long_num * sizeof(long *));
	nstar->byte_data = (unsigned char **)malloc(type_num.byte_num * sizeof(unsigned char *));
	
	type_num.double_num = 0;
	type_num.float_num = 0;
	type_num.byte_num = 0;
	type_num.int_num = 0;
	type_num.long_num = 0;
	type_num.short_num = 0;

	//get the information
	for(i = 0; i < col_tp; i++)
	{
		//printf("%s dddd\n",col_name[i]);
		//fits_get_colname(fptr,CASESEN, col_name[i], colname, &colnum, &status);
		//fits_get_coltype(fptr,  colnum, &typecode, &repeat, &width, &status);
		
		//col_type[i]=typecode;
		if(col_exist[i] == 1)
		{
			if(col_type[i] == TFLOAT)
			{
				//fits_read_tdim( fptr, i+1, 10, &naxis, naxes, &status);
				//printf("%d\n",i+1);
				//printf("%d naxe is %d %d\n",i+1,naxes[0],naxes[1]);
				//printf("naxe is %d\n",naxis);
				//for(j=0;j<col_array_num[i];j++)
				//{
				nstar -> float_data[type_num.float_num] = (float *)malloc(col_array_num[i]*nrows*sizeof(float));
				fits_read_col(fptr, col_type[i], i+1, 1, 1, col_array_num[i] * nrows, &enul, nstar -> float_data[type_num.float_num], &anynull, &status);
				//printf("%f\n",nstar->float_data[type_num.float_num][0]);
				//printf("%s ????\n",col_name[i]);
				if(!strcmp(col_name[i], "ALPHAWIN_J2000"))
				{
					rra = (double *)malloc((nrows + 2) * sizeof(double));
					for(kk = 0; kk < nrows; kk++)
					{
						rra[kk] = nstar -> float_data[type_num.float_num][kk];
					}
				}
				else if(!strcmp(col_name[i], "DELTAWIN_J2000"))
				{
					ddec = (double *)malloc((nrows + 2) * sizeof(double));
					for(kk = 0; kk < nrows; kk++)
					{
						ddec[kk] = nstar -> float_data[type_num.float_num][kk];
					}
				}
				else if(!strcmp(col_name[i], "MAG_PETRO"))
				{
					mag = (float *)malloc((nrows + 2) * sizeof(float));
					for(kk = 0; kk < nrows; kk++)
					{
						mag[kk] = nstar -> float_data[type_num.float_num][kk];
					}
				}
				else if(!strcmp(col_name[i], "MAGERR_PETRO"))
				{
					magerr = (float *)malloc((nrows + 2) * sizeof(float));
					for(kk = 0;kk < nrows; kk++)
					{
						magerr[kk] = nstar -> float_data[type_num.float_num][kk];
					}
				}
				type_num.float_num++;
				//}
			}
			else if(col_type[i] == TDOUBLE)
			{
				//for(j=0;j<col_array_num[i];j++)
				//{
					nstar -> double_data[type_num.double_num] = (double *)malloc(col_array_num[i] * nrows * sizeof(double));
					fits_read_col(fptr, col_type[i], i+1, 1, 1, col_array_num[i] * nrows, &dnul, nstar -> double_data[type_num.double_num], &anynull, &status);
					printf("%lf\n", nstar -> double_data[type_num.double_num][0]);
					printf("%s\n", col_name[i]);
					if(!strcmp(col_name[i], "ALPHAWIN_J2000"))
					{
						rra = (double *)malloc((nrows + 2) * sizeof(double));
						for(kk = 0; kk < nrows; kk++)
						{
							rra[kk] = nstar -> double_data[type_num.double_num][kk];
						}
					}
					else if(!strcmp(col_name[i], "DELTAWIN_J2000"))
					{
						ddec = (double *)malloc((nrows + 2) * sizeof(double));
						for(kk=0; kk < nrows; kk++)
						{
							ddec[kk] = nstar -> double_data[type_num.double_num][kk];
						}
					}
					else if(!strcmp(col_name[i], "MAG_PETRO"))
					{
						mag = (float *)malloc((nrows + 2) * sizeof(float));
						for(kk = 0; kk < nrows; kk++)
						{
							mag[kk] = nstar -> double_data[type_num.double_num][kk];
						}
					}
					else if(!strcmp(col_name[i], "MAGERR_PETRO"))
					{
						magerr = (float *)malloc((nrows + 2) * sizeof(float));
						for(kk = 0; kk < nrows; kk++)
						{
							magerr[kk] = nstar -> double_data[type_num.double_num][kk];
						}
					}
					type_num.double_num++;
				//}
			}
			else if(col_type[i] == TSHORT)
			{
				//for(j=0;j<col_array_num[i];j++)
				//{
					nstar -> short_data[type_num.short_num] = (short *)malloc(col_array_num[i] * nrows * sizeof(short));
					fits_read_col(fptr, col_type[i], i+1, 1, 1, col_array_num[i] * nrows, &inul, nstar -> short_data[type_num.short_num], &anynull, &status);
					type_num.short_num++;
				//}
			}
			else if(col_type[i] == TLONG)
			{
				//for(j=0;j<col_array_num[i];j++)
				//{
					nstar -> long_data[type_num.long_num] = (long *)malloc(col_array_num[i] * nrows * sizeof(long));
					fits_read_col(fptr, col_type[i], i+1, 1, 1, col_array_num[i] * nrows, &jnul, nstar -> long_data[type_num.long_num], &anynull, &status);
					type_num.long_num++;
				//}
			}
			else if(col_type[i] == TINT)
			{
				//for(j=0;j<col_array_num[i];j++)
				//{
					nstar -> int_data[type_num.int_num] = (int *)malloc(col_array_num[i] * nrows * sizeof(int));
					fits_read_col(fptr, col_type[i], i+1, 1, 1, col_array_num[i] * nrows, &knul, nstar -> int_data[type_num.int_num], &anynull, &status);
					type_num.int_num++;
				//}
			}
			else if(col_type[i] == TBYTE)
			{
				//for(j=0;j<col_array_num[i];j++)
				//{
					nstar -> byte_data[type_num.byte_num] = (unsigned char *)malloc(col_array_num[i] * nrows * sizeof(unsigned char));
					fits_read_col(fptr, col_type[i], i+1, 1, 1, col_array_num[i] * nrows, &bnul, nstar -> byte_data[type_num.byte_num], &anynull, &status);
					type_num.byte_num++;
				//}
			}
		}
	}
	
	fits_close_file(fptr, &status);
	
	col_need_num = col_tp;
	
 return ;
}

