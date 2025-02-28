import subprocess
import sys
import os
   
def test_sum_int():
    res = subprocess.run(["./build/app.exe"], input = "2+2", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 4
       
def test_mult_int():
    res = subprocess.run(["./build/app.exe"], input = "5*4", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 20
 
def test_many_actions_int():
    res = subprocess.run(["./build/app.exe"], input = "2+2*2+(8/2)", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 10

def test_many_actions_float():
    res = subprocess.run(["./build/app.exe","--float"], input = "6*5/(10+10)", text=True, capture_output=True)
    assert res.returncode == 0
    assert float(res.stdout) == 1.5000
    
def test_wrong_symbol():
    res = subprocess.run(["./build/app.exe"], input = "xx", text=True, capture_output=True)
    assert res.returncode != 0
    
def test_lot_space():
    res = subprocess.run(["./build/app.exe"], input = "1                                        *2", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 2
    
def test_lot_braces():
    res = subprocess.run(["./build/app.exe"], input = "((1+2)", text=True, capture_output=True)
    assert res.returncode != 0
    
def test_division_by_zero():
    res = subprocess.run(["./build/app.exe"], input = "1/0", text=True, capture_output=True)
    assert res.returncode != 0
    
def test_lot_symbols_in_expression():
    res = subprocess.run(["./build/app.exe"], input = "1++1", text=True, capture_output=True)
    assert res.returncode != 0
   
def test_max_number():
    res = subprocess.run(["./build/app.exe"], input = "2000000001+1", text=True, capture_output=True)
    assert res.returncode != 0

def test_un_minus():
    res = subprocess.run(["./build/app.exe"], input = "-2+1", text=True, capture_output=True)
    assert res.returncode != 0
def test_single_number_int():
    res = subprocess.run(["./build/app.exe"], input="42", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 42

def test_single_number_float():
    res = subprocess.run(["./build/app.exe", "--float"], input="42", text=True, capture_output=True)
    assert res.returncode == 0
    # В режиме float число 42 должно выводиться как 42.0000
    assert float(res.stdout) == 42.0000

def test_nested_parentheses_int():
    res = subprocess.run(["./build/app.exe"], input="((1+2)*(3+4))/5", text=True, capture_output=True)
    assert res.returncode == 0
    # (1+2)=3, (3+4)=7, 3*7=21, 21/5=4 (целочисленное деление)
    assert int(res.stdout) == 4

def test_complex_expression_float():
    res = subprocess.run(["./build/app.exe", "--float"], input="10+((20-3)*4)/7", text=True, capture_output=True)
    assert res.returncode == 0
    # (20-3)=17, 17*4=68, 68/7=9.7142857..., +10 = 19.7142857, округляется до 19.7143
    assert float(res.stdout) == 19.7143

def test_expression_with_extra_spaces():
    res = subprocess.run(["./build/app.exe"], input="   8    *   7   ", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 56

def test_multiple_parentheses_int():
    res = subprocess.run(["./build/app.exe"], input="((2+3)+((4+5)*2))", text=True, capture_output=True)
    assert res.returncode == 0
    # (2+3)=5, (4+5)=9, 9*2=18, 5+18=23
    assert int(res.stdout) == 23

def test_invalid_missing_operator():
    res = subprocess.run(["./build/app.exe"], input="2(3)", text=True, capture_output=True)
    assert res.returncode != 0
