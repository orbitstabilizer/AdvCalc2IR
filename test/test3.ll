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
  %reg_1 = xor i32 1, 5
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_1)
  %reg_2 = and i32 1, 5
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_2)
  %reg_3 = or i32 1, 5
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_3)
  %reg_4 = add i32 1, 1
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_4)
  %reg_5 = sub i32 1, 1
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_5)
  %reg_6 = sdiv i32 5, 2
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_6)
  %reg_7 = srem i32 4, 3
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_7)
  %reg_8 = mul i32 2, 4
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_8)
  %reg_9 = shl i32 1, 1
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_9)
  %reg_10 = ashr i32 4, 1
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_10)
  %a = alloca i32
  %reg_11 = call i32 @rr(i32 1, i32 1)
  store i32 %reg_11, i32* %a
  %reg_12 = load i32, i32* %a
  %reg_13 = ashr i32 %reg_12, 1
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_13)
  %b = alloca i32
  %reg_14 = shl i32 1, 31
  store i32 %reg_14, i32* %b
  %reg_15 = load i32, i32* %b
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_15)
  %reg_16 = load i32, i32* %a
  %reg_17 = load i32, i32* %b
  %reg_18 = and i32 %reg_16, %reg_17
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_18)
  %reg_19 = load i32, i32* %a
  %reg_20 = call i32 @lr(i32 %reg_19, i32 1)
  %reg_21 = sub i32 1, %reg_20
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_21)
  %reg_22 = mul i32 99, 2
  %reg_23 = add i32 %reg_22, 2
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_23)
  ret i32 0
}
