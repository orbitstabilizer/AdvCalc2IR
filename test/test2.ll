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
  %siu = alloca i32
  store i32 11, i32* %siu
  %siuuu = alloca i32
  store i32 7, i32* %siuuu
  %reg_1 = load i32, i32* %siu
  %reg_2 = load i32, i32* %siuuu
  %reg_3 = sdiv i32 %reg_1, %reg_2
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_3)
  %reg_4 = load i32, i32* %siu
  %reg_5 = load i32, i32* %siuuu
  %reg_6 = mul i32 %reg_4, %reg_5
  store i32 %reg_6, i32* %siu
  %reg_7 = load i32, i32* %siu
  %reg_8 = load i32, i32* %siu
  %reg_9 = sub i32 %reg_7, %reg_8
  %reg_10 = load i32, i32* %siu
  %reg_11 = load i32, i32* %siu
  %reg_12 = mul i32 %reg_10, %reg_11
  %reg_13 = load i32, i32* %siu
  %reg_14 = sdiv i32 %reg_12, %reg_13
  %reg_15 = add i32 %reg_9, %reg_14
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_15)
  ret i32 0
}
