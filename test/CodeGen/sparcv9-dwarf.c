// RUN: %clang_cc1 -triple sparcv9-unknown-unknown -emit-llvm %s -o - | FileCheck %s
static unsigned char dwarf_reg_size_table[102+1];

int test() {
  __builtin_init_dwarf_reg_size_table(dwarf_reg_size_table);

  return __builtin_dwarf_sp_column();
}

// CHECK-LABEL: define signext i32 @test()
// CHECK:       store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 0)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 1)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 2)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 3)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 4)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 5)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 6)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 7)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 8)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 9)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 10)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 11)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 12)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 13)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 14)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 15)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 16)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 17)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 18)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 19)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 20)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 21)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 22)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 23)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 24)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 25)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 26)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 27)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 28)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 29)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 30)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 31)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 32)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 33)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 34)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 35)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 36)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 37)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 38)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 39)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 40)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 41)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 42)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 43)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 44)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 45)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 46)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 47)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 48)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 49)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 50)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 51)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 52)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 53)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 54)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 55)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 56)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 57)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 58)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 59)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 60)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 61)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 62)
// CHECK-NEXT:  store i8 4, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 63)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 64)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 65)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 66)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 67)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 68)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 69)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 70)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 71)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 72)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 73)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 74)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 75)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 76)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 77)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 78)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 79)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 80)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 81)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 82)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 83)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 84)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 85)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 86)
// CHECK-NEXT:  store i8 8, i8* getelementptr inbounds ([103 x i8], [103 x i8]* @dwarf_reg_size_table, i64 0, i64 87)
// CHECK-NEXT:  ret i32 14
