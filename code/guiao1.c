#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operators.h"
#include "stack.h"
#include <math.h>
#include "macro.h"
#include "parser.h"
#include "alloca.h"

void sub(STACK *s)
{
    
    Container x = pop(s);
    Container y = pop(s);
    Container z;
    
    if (_Ylabel_ == 3 ||_Xlabel_ == 3)
    {
        x = toChar(x);
        y = toChar(y);
        _Zlabel_ = 3;
        _Zcar_ = _Ycar_-_Xcar_;
    }
    else if (_Ylabel_ == 1 ||_Xlabel_ == 1)
    {
        x = toDouble(x);
        y = toDouble(y);
        _Zlabel_ = 1;
        _ZnumD_ = _YnumD_-_XnumD_;
    }
    else{
        _Zlabel_ =_Xlabel_;
        _ZnumI_ = _YnumI_-_XnumI_;
    }
    push(s,z);
}

void realadd(STACK *s,Container x, Container y){
    Container z;
    if (_Ylabel_ == 3 ||_Xlabel_ == 3)
    {
        x = toChar(x);
        y = toChar(y);
        _Zlabel_ = 3;
        _Zcar_ = _Xcar_+_Ycar_;
        push(s,z);
    }
    else if (_Ylabel_ == 1 ||_Xlabel_ == 1)
    {
        x = toDouble(x);
        y = toDouble(y);
        _Zlabel_ = 1;
        _ZnumD_ = _XnumD_+_YnumD_;
        push(s,z);
    }
    else
    {
        _Zlabel_ =_Xlabel_;
        _ZnumI_ = _XnumI_+_YnumI_;
        push(s,z);
    }
}

void add4str(STACK *s,Container x, Container y)
{
    if(_Xlabel_ == 3){
        char aux[2];
        aux[0] = x.type.car;
        aux[1] = '\0';
        y.str = strcat(y.str,aux);
        push(s,y);

    }
    else{
        size_t l1 = strlen(y.str);
        size_t l2 = strlen(x.str);
        char* res = malloc(l1 + l2 + 1);
        memcpy(res,y.str,l1);
        memcpy(res+l1,x.str,l2+1);
        memcpy(y.str,res,l1+l2+1);
        free(res);
        push(s,y);
    }
}

void add(STACK *s){
    
    Container x = pop(s);
    Container y = pop(s);
    if (_Ylabel_ == 3 && _Xlabel_ == 4)
    {
        x.str[strlen(x.str)+1] = '\0';
        for (int i = strlen(x.str); i>0; i--)
        {
            x.str[i] = x.str[i-1];
        }
        x.str[0] = y.type.car;
        push(s,x);
    }
    else if(_Xlabel_ == 5 && _Ylabel_ != 5){
        prepush(x.a,y);
        push(s,x);
    }
    else if (_Ylabel_ == 5){
        arrcat(y.a,x);
        push(s,y);
        if(_Xlabel_ == 5){
            free(x.a->stack);
            free(x.a);
        }
    }
    else if(_Ylabel_ == 4){
        add4str(s,x,y);
    }
    else{
        realadd(s,x,y);
    }
}

void foldr(STACK *s, Container x, Container y, Container *vars){
    STACK *min = ministack();
    push(min,y.a->stack[1]);
    push(min,y.a->stack[2]);
    int i = 3;
    while (i <= y.a->sp)
    {
        char *helper = strdup(x.str);
        parser(min,helper,vars);
        push(min,y.a->stack[i]);
        i++;
        free(helper);
    }
    char *helper = strdup(x.str);
    parser(min,helper,vars);
    free(helper);
    Container z = min->stack[1];
    free(y.a->stack);
    free(y.a);
    push(s,z);
}

void realmult(STACK *s, Container x, Container y){
    Container z;
    if (_Ylabel_ == 1 ||_Xlabel_ == 1)
    {
        x = toDouble(x);
        y = toDouble(y);
        _Zlabel_ = 1;
        _ZnumD_ = _XnumD_*_YnumD_;
        push(s,z);
    }
    else
    {
        _Zlabel_ =_Xlabel_;
        _ZnumI_ = _XnumI_*_YnumI_;
        push(s,z);
    }
}

void mult(STACK *s, Container *vars)
{
    
    Container x = pop(s);
    Container y = pop(s);
    Container z;
    if (_Xlabel_ == 6)
    {
        foldr(s,x,y,vars);
    }
    else if (_Ylabel_ == 5)
    {
        Container aux;
        aux.label = 5;
        aux.a = new_stack();
        arrcat(aux.a,y);
        while (x.type.numI > 1)
        {
            arrcat(y.a,aux);
            x.type.numI--;
        } 
        push(s,y);
        free(aux.a->stack);
        free(aux.a);
    }
    else if(_Ylabel_ == 4)
    {
        z.label = 4;
        z.str = alloccStr();
        z.str = strcpy(z.str,y.str);
        while (_XnumI_ > 1)
        {
            z.str = strcat(z.str,y.str);
            _XnumI_--;
        }
        push(s,z);
    }
    else{
        realmult(s,x,y);
    }
}

void division(STACK *s)
{
    Container x = pop(s);
    Container y = pop(s);
    Container z;
    if (_Ylabel_ == 4)
    {
        z.label = 5;
        z.a = new_stack();
        char *token = alloca(BUFSIZ);
        while ((token = strtok_r(y.str,x.str,&y.str))!= NULL)
        {
            Container aux;
            aux.label = 4;
            aux.str = token;
            push(z.a,aux);
        }
    }
    else if (_Ylabel_ ==_Xlabel_ && _Xlabel_ == 2)
    {
        _Zlabel_ =_Xlabel_;
        _ZnumI_ = _YnumI_/_XnumI_;
    }
    else
    {
        x = toDouble(x);
        y = toDouble(y);
        _Zlabel_ = 1;
        _ZnumD_ = _YnumD_/_XnumD_;
    }
    push(s,z);
}

// ) guião 4
Container rmarr2(STACK *s, Container x) {
    /* int i;
    Container y;
    y.label = 5;
    y.a = new_stack();
    STACK *aux = x.a;
    for(i = 1; i < aux->sp; i++) {
        Container z = aux->stack[i];
        push(y.a,z);
    }
    Container z = pop(aux);
    while(aux->sp >= 1) pop(aux);
    push(s,y);
    return z; */
    Container z = pop(x.a);
    push(s,x);
    return z;
}

void add1(STACK *s)
{
    Container y = pop(s);
    if (_Ylabel_ == 4) 
    {
        Container x;
        x.label = 3;
        x.type.car = y.str[strlen(y.str)-1];
        y.str[strlen(y.str)-1] = '\0';
        push(s,y);
        push(s,x);
    }
    else if(_Ylabel_ == 1){
        _YnumD_++;
        push(s,y);
    }
    else if(_Ylabel_ == 2){
        _YnumI_++;
        push(s,y);
    }
    else if(_Ylabel_ == 5) {
        Container z = rmarr2(s,y);
        push(s,z);
    }
    else {
        _Ycar_++;
        push(s,y);
    }
}

// ( guião 4
Container rmarr1(STACK *s, Container x) {
    int i;
    Container y;
    y.label = 5;
    y.a = new_stack();
    STACK *aux = x.a;
    for(i = 2; i <= aux->sp; i++) {
        Container z = aux->stack[i];
        push(y.a,z);
    }
    Container z = aux->stack[1];
    while(aux->sp >= 1) pop(aux);
    push(s,y);
    return z;
}

void sub1(STACK *s)
{
    Container y = pop(s);
    if (_Ylabel_ == 4) 
    {
        Container x;
        x.label = 3;
        x.type.car = y.str[0];
        y.str++;
        push(s,y);
        push(s,x);
    }
    else if(_Ylabel_ == 1){
        _YnumD_--;
        push(s,y);
    }
    else if(_Ylabel_ == 2){
        _YnumI_--;
        push(s,y);
    }
    else if(_Ylabel_ == 5) {
        Container z = rmarr1(s,y);
        push(s,z);
    }
    else {
        _Ycar_--;
        push(s,y);
    }
}

void mod(STACK *s, Container *vars)
{   
    Container x = pop(s);
    Container y = pop(s);
    Container z;
    if (_Xlabel_ == 6)
    {
        STACK *min = new_stack();
        if (_Ylabel_ == 5)
        {   
            for (int i = 1; i <= y.a->sp;i++)
            {
                push(min,y.a->stack[i]);
                char *helper = strdup(x.str);
                parser(min,helper,vars);
                free(helper);
            }
            free(y.a->stack);
            free(y.a);
            y.a = min;
            push(s,y);
        }
        else
        {
            z.label = 3;
            char *str = strdup(y.str);
            for (int i = 0; str[i] != '\0';i++)
            {
                z.type.car = str[i];
                char *helper = strdup(x.str);
                push(min,z);
                parser(min,helper,vars);
                y.str[i] = min->stack[1].type.car;
                pop(min);
                free(helper);

            }
            push(s,y);
        }
    }
    else
    {
        x = toInt(x);
        y = toInt(y);
        _Zlabel_ = 2;
        _ZnumI_= _YnumI_%_XnumI_;
        push(s,z);
    }
}
size_t forloop(Container x,Container y, short *val){
    size_t i = 0;
    for (; i < strlen(x.str) && *val; i++)
    {
        if (y.str[i] != x.str[i])
        {
            *val = 0;
        }
    }
    return i;
}

void expo(STACK *s)
{
    Container x = pop(s);
    Container y = pop(s);
    Container z;
    if (_Ylabel_ == 4)
    {
        long pos = 0;
        short val = 1;
        short end = 1;
        while (y.str[0] != '\0' && end)
        {
            if (y.str[0] == x.str[0])
            {
                size_t i = 0;
                val = 1;
                i = forloop(x,y,&val);
                if(i == strlen(x.str)){
                    end = 0;
                }
            }
            pos++;
            y.str++;
        }
        if (end)
        {
            pos = 0;
        }
        z.label = 2;
        z.type.numI = --pos;
        push(s,z);
    }
    else{
        x = toDouble(x);
        y = toDouble(y);
        _Zlabel_ = 1;
        _ZnumD_ = pow(_YnumD_,_XnumD_);
        push(s,z);
    }
}

void e_bit(STACK *s)
{
    Container x = pop(s);
    Container y = pop(s);
    Container z;
    x = toInt(x);
    y = toInt(y);
    _Zlabel_ = 2;
    _ZnumI_ = _YnumI_&_XnumI_;
    push(s,z);
}

void ou_bit(STACK *s)
{
    Container x = pop(s);
    Container y = pop(s);
    Container z;
    x = toInt(x);
    y = toInt(y);
    _Zlabel_ = 2;
    _ZnumI_ = _YnumI_|_XnumI_;
    push(s,z);
}

void xor_bit(STACK *s)
{
    Container x = pop(s);
    Container y = pop(s);
    Container z;
    x = toInt(x);
    y = toInt(y);
    _Zlabel_ = 2;
    _ZnumI_ = _YnumI_^_XnumI_;
    push(s,z);

}

void not_bit(STACK *s, Container *vars)
{
    Container x = pop(s);
    pop(s);
    if(_Xlabel_ == 6){
        Container y = pop(s);
        STACK *min = ministack();
        push(min,y);
        char *helper = strdup(x.str);
        parser(min,helper,vars);
        push(s,min->stack[1]);
        free(min->stack);
        free(min);
    }
    else if (_Xlabel_ == 5)
    {
        for(int i = 1; i <= x.a->sp; i++) {
        push(s,x.a->stack[i]);
        }
    }
    else if(s->sp == 1){
            x = toInt(x);
            _XnumI_ = ~_XnumI_;
            push(s,x);
        }
    else{
        Container y = pop(s);
        if(_Ylabel_==5){
            int i;
            STACK *aux = x.a;
            for(i = 0; i <= aux->sp; i++) {
                Container y = aux->stack[i];
                push(s,y);
            }
        }
        else{
            x = toInt(x);
            _XnumI_ = ~_XnumI_;
            push(s,x);
        }

        }
    }

void truthy(STACK *s, Container *vars){
    Container x = pop(s);
    Container y = pop(s);
    STACK *min = new_stack();
    Container z;
    push(min,y);
    while (y.type.numI != 0)
    {
        z = min->stack[1];
        char *helper = strdup(x.str);
        parser(min,helper,vars);
        free(helper);
    }
    push(s,z);
}
