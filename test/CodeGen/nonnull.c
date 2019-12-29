// RUN: %clang_cc1 -triple x86_64-apple-darwin -emit-llvm < %s | FileCheck -check-prefix=NULL-INVALID %s
// RUN: %clang_cc1 -triple x86_64-apple-darwin -emit-llvm -fno-delete-null-pointer-checks < %s | FileCheck -check-prefix=NULL-VALID %s

// NULL-INVALID: define void @foo(i32* nonnull %x)
// NULL-VALID: define void @foo(i32* %x)
void foo(int * __attribute__((nonnull)) x) {
  *x = 0;
}

// NULL-INVALID: define void @bar(i32* nonnull %x)
// NULL-VALID: define void @bar(i32* %x)
void bar(int * x) __attribute__((nonnull(1)))  {
  *x = 0;
}

// NULL-INVALID: define void @bar2(i32* %x, i32* nonnull %y)
// NULL-VALID: define void @bar2(i32* %x, i32* %y)
void bar2(int * x, int * y) __attribute__((nonnull(2)))  {
  *x = 0;
}

static int a;
// NULL-INVALID: define nonnull i32* @bar3()
// NULL-VALID: define i32* @bar3()
int * bar3() __attribute__((returns_nonnull))  {
  return &a;
}

// NULL-INVALID: define i32 @bar4(i32 %n, i32* nonnull %p)
// NULL-VALID: define i32 @bar4(i32 %n, i32* %p)
int bar4(int n, int *p) __attribute__((nonnull)) {
  return n + *p;
}

// NULL-INVALID: define i32 @bar5(i32 %n, i32* nonnull %p)
// NULL-VALID: define i32 @bar5(i32 %n, i32* %p)
int bar5(int n, int *p) __attribute__((nonnull(1, 2))) {
  return n + *p;
}

typedef union {
  unsigned long long n;
  int *p;
  double d;
} TransparentUnion __attribute__((transparent_union));

// NULL-INVALID: define i32 @bar6(i64 %
// NULL-VALID: define i32 @bar6(i64 %
int bar6(TransparentUnion tu) __attribute__((nonnull(1))) {
  return *tu.p;
}

// NULL-INVALID: define void @bar7(i32* nonnull %a, i32* nonnull %b)
// NULL-VALID: define void @bar7(i32* %a, i32* %b)
void bar7(int *a, int *b) __attribute__((nonnull(1)))
__attribute__((nonnull(2))) {}

// NULL-INVALID: define void @bar8(i32* nonnull %a, i32* nonnull %b)
// NULL-VALID: define void @bar8(i32* %a, i32* %b)
void bar8(int *a, int *b) __attribute__((nonnull))
__attribute__((nonnull(1))) {}
