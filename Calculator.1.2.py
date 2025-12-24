# Python Project 1.0
# Calculator Module Version 1.2 

#Imports
import math
import time

# Introduction to the Calculator, Greeting and initial startup
print('')
print('Calculator Module, Version 1.2')
print('')
time.sleep(1.0)

# Loop Setup
Cont = 'yes'
while Cont == 'yes':
    
# Functions Variable Setup
    print('A) Addition')
    time.sleep(0.25)
    print('B) Subtraction')
    time.sleep(0.25)
    print('C) Multiplication')
    time.sleep(0.25)
    print('D) Division')
    time.sleep(0.25)
    print('E) Exponential')
    time.sleep(0.25)
    print('F) Root Function')
    time.sleep(0.25)
    print('-'*60)
    func = input('Choose one of the following Options: ').upper()
    
# Mathematics Portion One: Addition
    time.sleep(0.5)
    if func == 'A':
        print('ADDITION')
        print('')
        print('ENTER: ')
        firstDigit = input('>>> ')
        secondDigit = input('>>> ')
        answer = float(firstDigit) + float(secondDigit)
        time.sleep(0.25)
        print('')
        print(str(firstDigit)+' + '+str(secondDigit)+' = '+str(answer))
        print('')
        print('-'*60)
# Mathematics Portion One : Addition, RERUN OPERATION
        Cont = input('Would you like to perform another function? yes or no: ').lower()
        if Cont == 'no':
            print('')
            print('Thank you for using Calculator Module, Version 1.2')
            print('')
            print('-'*60)

# Mathematics Portion Two : Subtraction
    elif func == 'B':
        print('SUBTRACTION')
        print('')
        print('ENTER: ')
        firstDigit = input('>>> ')
        secondDigit = input('>>> ')
        answer = float(firstDigit) - float(secondDigit)
        time.sleep(0.25)
        print('')
        print(str(firstDigit)+' - '+str(secondDigit)+' = '+str(answer))
        print('')
        print('-'*60)
# Mathematics Portion Two : Subtraction, RERUN OPERATION
        Cont = input('Would you like to perform another function? yes or no: ').lower()
        if Cont == 'no':
            print('')
            print('Thank you for using Calculator Module, Version 1.2')
            print('')
            print('-'*60)

# Mathematics Portion Three : Multiplication
    elif func == 'C':
        print('MULTIPLICATION')
        print('')
        print('ENTER')
        firstDigit = input('>>> ')
        secondDigit = input('>>> ') 
        answer = float(firstDigit) * float(secondDigit)
        time.sleep(0.25)
        print('')
        print(str(firstDigit)+' x '+str(secondDigit)+' = '+str(answer))
        print('')
        print('-'*60)
# Mathematics Portion Three : Multiplication, RERUN OPERATION
        Cont = input('Would you like to perform another function? yes or no: ').lower()
        if Cont == 'no':
            print('')
            print('Thank you for using Calculator Module, Version 1.2')
            print('')
            print('-'*60)

# Mathematics Portion Four : Division 
    elif func == 'D':
        print('DIVISION')
        print('')
        print('ENTER')
        firstDigit = input('>>> ')
        secondDigit = input('>>> ')
        answer = float(firstDigit) / float(secondDigit)
        answer_decimal = float("{:.2f}".format(answer))
        time.sleep(0.25)
        print('')
        print(str(firstDigit)+' / '+str(secondDigit)+' = '+str(answer_decimal)) #+'Remainder: '+math.remainder(float(firstDigit), float(secondDigit)))
        print('')
        print('-'*60)
# Mathematics Portion Four : Division, RERUN OPERATION
        Cont = input('Would you like to perform another function? yes or no: ').lower()
        if Cont == 'no':
            print('')
            print('Thank you for using Calculator Module, Version 1.2')
            print('')
            print('-'*60)

# Mathematics Portion Five : Exponents
    elif func == 'E':
        print('EXPONENT')
        print('')
        print('ENTER')
        firstDigit = input('>>> ')
        secondDigit = input('>>> ')
        answer = float(firstDigit) ** float(secondDigit)
        time.sleep(0.25)
        print('')
        print(str(firstDigit)+' ^ '+str(secondDigit)+' = '+str(answer))
        print('')
        print('-'*60)
# Mathematics Portion Five : Exponents, RERUN OPERATION
        Cont = input('Would you like to perform another function? yes or no: ').lower()
        if Cont == 'no':
            print('')
            print('Thank you for using Calculator Module, Version 1.2')
            print('')
            print('-'*60)

# Mathematics Portion Six : Root Functions
    elif func == 'F':
        print('ROOT FUNCTION')
        print('')
        print('ENTER')
        firstDigit = input('>>> ')
        answer = (math.sqrt(float(firstDigit)))
        answer_decimal = float("{:.2f}".format(answer))
        time.sleep(0.25)
        print('')
        print('sqrt/'+str(firstDigit)+' = '+str(answer_decimal))
        print('')
        print('-'*60)
# Mathematics Portion Six : Root Functions, RERUN OPERATION
        Cont = input('Would you like to perform another function? yes or no: ').lower()
        if Cont == 'no':
            print('')
            print('Thank you for using Calculator Module, Version 1.2')
            print('')
            print('-'*60)