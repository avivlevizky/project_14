#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * base4 (int i, int j, int active) {  /* i - number to convert, j - how many digits , active - reverse or not*/
    int res;
    int k;
    int f;
    int length;
    char * temp;
    char * temp1;
    char * ans ;
    k = 0;

    if (i == 0){
        ans = (char *)malloc(sizeof(char)*(j+1));
        if (!ans) {
            printf("\ncannot allocate memory for base4 function\n");
            printf("\npreform exit\n");
            exit(0);}
        ans[0] = 'a';
        for(f = 1; f < j; f++){
            ans[f]='a';

        }
        ans[f] = '\0';
        return ans;


    }

    ans = (char *)malloc(sizeof(char)*(k+1));
    if (!ans) {
        printf("\ncannot allocate memory for base4 function\n");
        printf("\npreform exit\n");
        exit(0);
    }

    while (i > 0){                          /*Creating to base4 string*/
        res = i%4;
        i=i/4;
                                            /*converting process*/
        if(res == 0){ans[k++] = 'a';}
        else if(res == 1){ans[k++] = 'b';}
        else if(res == 2){ans[k++] = 'c';}
        else if(res == 3){ans[k++] = 'd';}


        temp = (char*)realloc(ans,(k+1));  /*increasing the string dynamically*/
        if (!temp) {
            printf("\ncannot allocate memory for base4 function");
            printf("preform exit\n");
            exit(0);
        }
        else
        {
            ans = temp;

        }
    }

    if (active)                             /*indicate if reverse the base4 code or not*/
    {
        temp = (char*)realloc(ans,(k+1));
        if (!temp) {
            printf("\ncannot allocate memory for base4 function");
            printf("preform exit\n");
            exit(0);
        }
        else
        {
            ans = temp;

        }


        temp = (char *)malloc(sizeof(char)*(k+1));
        for(f = 0; f < k; f++)
        {
            temp[f] = ans[(k-1)-f];
        }
        temp[k]='\0';
        free(ans);
        ans = temp;

    }

    length = strlen(ans);
    if (j > length){                        /*padding the string with the requested amount of a's*/
        temp1 = (char *)malloc(sizeof(char)*(j+1));
        for (f = 0; f < j; f++)
        {
            if (f <(j-length))
            {
                temp1[f] = 'a';
            }
            else
            {
                temp1[f] = temp[f-(j-length)];
            }

        }
            free(ans);
            return temp1;
    }
    return ans;
}


int main ( ){

    char *p;

    p = base4(100,8,1);
    printf("\n%s\n",p);


    return 1;

}









