#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
     
#include "Cross.h"
#include "ArrayWriter.h"
#include "ImageArray.h"

ArrayWriters writers;
StdFileIO* outputPtr;

void show_help(const char* prog_name)
{
    printf("%s [--orientation=vert-msb*|vert-lsb|horiz-msb|horiz-lsb] [--left2right=0|1*] [--top2bottom=0|1*] [--writer=#] [--invert=0|1*] [--force8bit=0*|1] <inputfile> <outputfile>\n", prog_name);
    printf("\tWhere # is the writer type:\n");
    for(unsigned int i = 0; i < writers.size(); i++)
        printf("\t\t%2u : %s%s\n",i,writers[i]->GetName(),i==0?"*":"");
    printf("Defaults are shown by *\n");
    printf("Orientation is only meaningful for 1 bit/pixel images\n");
}

int choose_orientation(const char* o)
{
    if (strcmp("vert-msb",o) == 0)
        return 0;
    else if (strcmp("vert-lsb",o) == 0)
        return 1;
    else if (strcmp("horiz-msb",o) == 0)
        return 2;
    else if (strcmp("horiz-lsb",o) == 0)
        return 3;
    else
        return -1;
}

int main(int argc, char* argv[], char** env)
{
    int ret = 0;
    int c;

    unsigned int orientation = 0;
    bool left2right = true;
    bool top2bottom = true;
    bool pad_msb = true;
    bool invert = true;
    bool force8bit = false;
    unsigned int writerid;

    const char* infilename;
    const char* outfilename;
    
    while (1) {
        static struct option long_options[] = {
            {"orientation", required_argument, 0, 'o'},
            {"left2right", required_argument, 0, 'l'},
            {"top2bottom", required_argument, 0, 't'},
            {"writer", required_argument, 0, 'w'},
            {"invert", required_argument, 0, 'i'},
            {"force8bit", required_argument, 0, '8'},
            {"help", no_argument, 0, '?'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, "o:l:t:i?",
                         long_options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        
        switch (c) {
        case 'o':
            orientation = choose_orientation(optarg);
            if (orientation >= 0) {
                printf("orientation: %s\n", optarg);
            }
            else {
                show_help(argv[0]);
                exit(1);
            }
            break;
            
        case 'l':
            left2right = atoi(optarg) != 0;
            printf("left2right: %d\n",  left2right);
            break;
            
        case 't':
            top2bottom = atoi(optarg) != 0;
            printf("top2bottom: %d\n", top2bottom);
            break;

        case 'w':
            writerid = atoi(optarg);
            printf("writer: %d\n", writerid);
            break;
            
        case 'i':
            invert = atoi(optarg) != 0;
            printf("invert: %d\n", invert);
            break;
            
        case '8':
            force8bit = atoi(optarg) != 0;
            printf("force8bit: %d\n", force8bit);
            break;
            
        case '?':
            show_help(argv[0]);
            exit(1);
            break;
            
        default:
            show_help(argv[0]);
            exit(1);
        }
    }
    
    /* Print any remaining command line arguments (not options). */
    if (argc - optind  == 2) {
        infilename = argv[optind];
        outfilename = argv[optind+1];

        printf("In file: %s\n",infilename);
        printf("Out file: %s\n",outfilename);
    }
    else {
        show_help(argv[0]);
        exit(1);
    }
    
    Bitmap bitmap;
    if (bitmap.Load(infilename)) {

        FILE* outfile = fopen(outfilename, "wb");
        if (outfile) {
            StdFileIO fileio(fileno(outfile));            
            outputPtr = &fileio;
            
            ImageArray img(&bitmap);
            ImageArray::BitPacker* packer = NULL;

            int bpp = force8bit?8:bitmap.GetBPP();          
            switch (bpp) {
            case 1:
                packer = new BitPack1Bit(!left2right,!top2bottom,orientation,pad_msb);
                break;
            case 8:
                packer = new BitPack8Bit(!left2right,!top2bottom);
                break;
            default:
                fprintf(stderr,"Invalid number of bits/pixel: %u", bitmap.GetBPP());
                ret = -3;
                break;
            }
            
            if (packer) {
                img.Ready(packer,invert);
                
                if (writerid >= 0 && static_cast<unsigned int>(writerid) < writers.size()) {
                    writers[writerid]->Writer(&img);
                }
                else {
                    fprintf(stderr,"Invalid array writer #%u", writerid);
                    ret = -4;
                }
                
                delete packer;
            }
            
            fclose(outfile);
        }
        else {
            fprintf(stderr, "Opening %s failed\n", outfilename);
            ret = -1;
        }
    }
    else {
        fprintf(stderr, "Loading %s failed\n", infilename);
        ret = -2;
    }

    return ret;
};
