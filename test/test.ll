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
  %reg_1 = srem i32 4, 5
  store i32 %reg_1, i32* %a
  %b = alloca i32
  %reg_2 = sdiv i32 4, 2
  store i32 %reg_2, i32* %b
  %reg_3 = load i32, i32* %a
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_3)
  %reg_4 = load i32, i32* %b
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_4)
  %x = alloca i32
  store i32 3, i32* %x
  %y = alloca i32
  store i32 31, i32* %y
  %reg_5 = load i32, i32* %x
  %reg_6 = load i32, i32* %y
  %reg_7 = add i32 %reg_5, %reg_6
  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_7)
  %reg_8 = call i32 @not(i32 1)
  %reg_9 = xor i32 1, 0
  %reg_10 = add i32 %reg_8, %reg_9
  store i32 %reg_10, i32* %x
  ret i32 0
}
