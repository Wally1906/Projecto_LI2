#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operators.h"
#include "stack.h"
#include <math.h>
#include "type_changes.h"
#include "macro.h"

#define _TRUE_      \
    Container True; \
    _Truelabel_ = 2;\
    _TruenumI_ = 1; \
    push(s,True); 

void coloca(STACK *s, Container *vars, char *token){
   Container x = pop(s);
    int ascii = token[1];
    if(_Xlabel_ == 5){
        vars[ascii-65].label = 5;
        vars[ascii-65].a = new_stack();
        arrcat(vars[ascii-65].a,x);
        push(s,x);
    }
    else{
       vars[ascii-65] = x; 
       push(s,x); 
    }
}

void ifelse(STACK *s){
    Container z = pop(s);
    Container y = pop(s);
    Container x = pop(s);
    if(_Ylabel_ == 4 && _Zlabel_ == 4)
    {
        if(isZero(x)) push(s,y);
        else push(s,z);
    }
    else if(_Ylabel_ == 5 && _Zlabel_ == 5)
    {
        if(isZero(x)) push(s,y);
        else push(s,z);
    }
    else if(isZero(x)) push(s,y);
    else push(s,z);
}

void igual(STACK *s){
    Container y = pop(s);
    Container x = pop(s);
    if(_Xlabel_ == 4 && _Ylabel_ == 4)
    {
        if(strcmp(x.str,y.str) == 0){
            Container True;
            _Truelabel_ = 2;
            _TruenumI_ = 1;
            push(s,True);
        }
        else
        {
            Container False;
            _Falselabel_ = 2;
            _FalsenumI_ = 0;
            push(s,False);
        }
    }
    else if(_Xlabel_ == 4){
        Container z;
        z.label = 3;
        z.type.car = x.str[y.type.numI];
        push(s,z);
    }
    else if(_Xlabel_ == 5){
        push(s,x.a->stack[(y.type.numI)+1]);
    }
    else{
        if(comparaCont(x,y) == 1)
        {
            Container True;
            _Truelabel_ = 2;
            _TruenumI_ = 1;
            push(s,True);
        }
        else if(_Ylabel_ == 5 ||_Xlabel_ == 5){
            int w = pop(s).type.numI;
            STACK *aux = y.a;
            Container z = aux->stack[w+1];
            push(s,z);
        }
        else
        {
            Container False;
            _Falselabel_ = 2;
            _FalsenumI_ = 0;
            push(s,False);
        }
    }
}

void strmaior(STACK *s,Container x, Container y)
{
    if(strcmp(x.str,y.str) > 0){
            Container True;
            _Truelabel_ = 2;
            _TruenumI_ = 1;
            push(s,True);
        }
        else
        {
            Container False;
            _Falselabel_ = 2;
            _FalsenumI_ = 0;
            push(s,False);
        }
}

void maior(STACK *s){
    Container y = pop(s);
    Container x = pop(s);
    if(_Xlabel_ == 4 && _Ylabel_ == 4){
        strmaior(s,x,y);
    }
    else if(_Xlabel_ == 5 && _Ylabel_ == 5){
        if(x.a->sp > y.a->sp)
        {
            Container True;
            _Truelabel_ = 2;
            _TruenumI_ = 1;
            push(s,True);
        }
        else
        {
            Container False;
            _Falselabel_ = 2;
            _FalsenumI_ = 0;
            push(s,False);
        }
    }
    else if(_Xlabel_ == 4){
        x.str += (strlen(x.str)-y.type.numI);
        push(s,x);
    }
    else if(_Xlabel_ == 5){
        Container z;
        z.label = 5;
        z.a = new_stack();
        while (y.type.numI>0)
        {
            y.type.numI--;
            prepush(z.a,pop(x.a));
        }
        push(s,z);
    }
    else{
        x = toDouble(x);
        y = toDouble(y);
        if(comparaCont(x,y) == 2)
        {
            Container True;
            _Truelabel_ = 2;
            _TruenumI_ = 1;
            push(s,True);
        }
        else
        {
            Container False;
            _Falselabel_ = 2;
            _FalsenumI_ = 0;
            push(s,False);
        }
    }   
}

void popmachine(STACK *x , int f){
    while (f>0)
            {
                f--;
                pop(x);
            }
}

void strmenor(STACK *s,Container x, Container y)
{
    if(strcmp(x.str,y.str) < 0){
            Container True;
            _Truelabel_ = 2;
            _TruenumI_ = 1;
            push(s,True);
        }
        else
        {
            Container False;
            _Falselabel_ = 2;
            _FalsenumI_ = 0;
            push(s,False);
        }
}

void menor(STACK *s){
    Container y = pop(s);
    Container x = pop(s);
    if(_Xlabel_ == 4 && _Ylabel_ == 4)
    {
        strmenor(s,x,y);
    }
    else if(_Xlabel_ == 5 && _Ylabel_ == 5){
        if(x.a->sp < y.a->sp)
        {
            _TRUE_
        }
        else
        {
            Container False;
            _Falselabel_ = 2;
            _FalsenumI_ = 0;
            push(s,False);
        }
    }
    else if(_Xlabel_ == 4)
    {
        Container z;
        z.label = 4;
        z.str = strndup(x.str,y.type.numI);
        push(s,z);
    }
    else if(_Xlabel_ == 5){
        int f = x.a->sp-y.type.numI;
        if(x.a->sp-y.type.numI == 0) push(s,x);
        else{
            popmachine(x.a,f);
            push(s,x);
            }
        }
    else{
        x = toDouble(x);
        y = toDouble(y);
        if(comparaCont(x,y) == 3)
        {
            _TRUE_
        }
        else
        {
            Container False;
            _Falselabel_ = 2;
            _FalsenumI_ = 0;
            push(s,False);
        }
    }
}
void eAnd(STACK *s){
    Container y = pop(s);
    Container x = pop(s);
    if (isZero(x) && isZero(y))
    {
        push(s,y);
    }
    else{
        Container False;
        _Falselabel_ = 2;
        _FalsenumI_ = 0;
        push(s,False);
    }
}

void eOr(STACK *s){
    Container y = pop(s);
    Container x = pop(s);
    if (isZero(x))
    {
        push(s,x);
    }
    else if(isZero(y))
    {
        push(s,y);
    }
    else{
        Container False;
        _Falselabel_ = 2;
        _FalsenumI_ = 0;
        push(s,False);
    }
}

void eMaior(STACK *s){
    Container y = pop(s);
    Container x = pop(s);
    if(_Xlabel_ == 4 && _Ylabel_ == 4)
    {
        if(strcmp(x.str,y.str) > 0)      
            push(s,x);
        else
            push(s,y);
    }
    else if(_Xlabel_ == 5 && _Ylabel_ == 5){
        if(x.a->sp > y.a->sp)
        {
            push(s,x);
        }
        else
        {
            push(s,y);
        }
    }
    else if(comparaCont(x,y) == 3)
    {
        push(s,y);
    }
    else
    {
        push(s,x);
    }
}

void eMenor(STACK *s){
    Container y = pop(s);
    Container x = pop(s);
    if(_Xlabel_ == 4 && _Ylabel_ == 4)
    {
        if(strcmp(x.str,y.str) < 0)      
            push(s,x);
        else
            push(s,y);
    }
    else if(_Xlabel_ == 5 && _Ylabel_ == 5){
        if(x.a->sp < y.a->sp)
        {
            push(s,x);
        }
        else
        {
            push(s,y);
        }
    }
    else if(comparaCont(x,y) == 3)
    {
        push(s,x);
    }
    else
    {
        push(s,y);
    }
}

void nots(STACK *s){
    Container x = pop(s);
    x = toDouble(x);
    if (!isZero(x))
    {
        Container True;
        _Truelabel_ = 2;
        _TruenumI_ = 1;
        push(s,True);
    }
    else
    {
        Container False;
        _Falselabel_ = 2;
        _FalsenumI_ = 0;
        push(s,False);
    }
}