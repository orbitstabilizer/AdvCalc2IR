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
  %a = alloca i32
  %reg_1 = xor i32 1, 4
  %reg_2 = shl i32 1, 4
  %reg_3 = sdiv i32 %reg_1, %reg_2
  %reg_4 = mul i32 %reg_3, 4
  store i32 %reg_4, i32* %a
  %reg_5 = xor i32 1, 4
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_5)
  %reg_6 = shl i32 1, 4
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_6)
  %reg_7 = add i32 94, 6
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_7)
  %b = alloca i32
  store i32 0, i32* %b
  store i32 31, i32* %a
  %reg_8 = load i32, i32* %a
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_8)
  %reg_9 = load i32, i32* %a
  %reg_10 = call i32 @rr(i32 %reg_9, i32 1)
  store i32 %reg_10, i32* %a
  %reg_11 = load i32, i32* %a
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_11)
  %reg_12 = load i32, i32* %a
  %reg_13 = call i32 @lr(i32 %reg_12, i32 1)
  store i32 %reg_13, i32* %a
  %reg_14 = load i32, i32* %a
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_14)
  %reg_15 = sub i32 0, 1
  %reg_16 = call i32 @not(i32 %reg_15)
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_16)
  ret i32 0
}
