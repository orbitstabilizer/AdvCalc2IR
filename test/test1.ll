; ModuleID = 'advcalc2ir'

declare i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @lr(i32 %x, i32 %c) {
  %l1 = shl i32 %x, %c 
  %l2 = sub i32 32, %c 
  %l3 = lshr i32 %x, %l2 
  %l4 = or i32 %l1, %l3 
  ret i32 %l4 
}
define i32 @rr(i32 %x, i32 %c) {
  %l1 = lshr i32 %x, %c 
  %l2 = sub i32 32, %c 
  %l3 = shl i32 %x, %l2 
  %l4 = or i32 %l1, %l3 
  ret i32 %l4 
}
define i32 @not(i32 %x) {
  %n1 = xor i32 %x, -1 
  ret i32 %n1 
}

define i32 @main() {
  %x = alloca i32
  store i32 3, i32* %x
  %y = alloca i32
  store i32 5, i32* %y
  %zvalue = alloca i32
  %reg_1 = load i32, i32* %x
  %reg_2 = load i32, i32* %y
  %reg_3 = add i32 1, %reg_2
  %reg_4 = mul i32 %reg_1, %reg_3
  %reg_5 = add i32 23, %reg_4
  store i32 %reg_5, i32* %zvalue
  %reg_6 = load i32, i32* %zvalue
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_6)
  %k = alloca i32
  %reg_7 = load i32, i32* %x
  %reg_8 = load i32, i32* %y
  %reg_9 = sub i32 %reg_7, %reg_8
  %reg_10 = load i32, i32* %zvalue
  %reg_11 = sub i32 %reg_9, %reg_10
  store i32 %reg_11, i32* %k
  %reg_12 = load i32, i32* %x
  %reg_13 = load i32, i32* %y
  %reg_14 = mul i32 3, %reg_13
  %reg_15 = add i32 2, 5
  %reg_16 = mul i32 1, %reg_15
  %reg_17 = mul i32 %reg_14, %reg_16
  %reg_18 = add i32 %reg_12, %reg_17
  store i32 %reg_18, i32* %k
  %reg_19 = load i32, i32* %k
  %reg_20 = add i32 %reg_19, 1
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_20)
  ret i32 0
}
