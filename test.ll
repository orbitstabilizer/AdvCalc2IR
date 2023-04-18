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
  store i32 1, i32* %x  ; x = 1
  %y = alloca i32
  store i32 2, i32* %y  ; y = 1

  %reg_1 = load i32, i32* %x 
  %reg_2 = load i32, i32* %y  
  %reg_3 = call i32 @rr(i32 %reg_1, i32 %reg_2)

  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_3)

  ret i32 0
}


