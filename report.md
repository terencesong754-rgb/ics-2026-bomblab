# Bomb Lab 实验报告 
- 姓名：宋翼如
- 学号：25300120083

## 目录
- [Bomb Lab 实验报告](#bomb-lab-实验报告)
  - [目录](#目录)
  - [Tutor与前置学习](#tutor与前置学习)
    - [Tutor实验pahse 0](#tutor实验pahse-0)
    - [前置知识准备](#前置知识准备)


## Tutor与前置学习
仔细阅读tutor中的**tutor 教学：从寄存器到通用 solver**并跟随教学中的指示，完成了前置教学实验。
学习了使用`gdb` `objdump`的静态与动态调试，并熟悉相关命令行。

## phase 1
反汇编phase01，得到如下编译码：
```
0000000000402020 <phase_01>:
  402020:       f3 0f 1e fa             endbr64
  402024:       55                      push   %rbp
  402025:       48 89 e5                mov    %rsp,%rbp
  402028:       41 54                   push   %r12
  40202a:       49 89 f4                mov    %rsi,%r12
  40202d:       be 01 00 00 00          mov    $0x1,%esi
  402032:       53                      push   %rbx
  402033:       48 89 fb                mov    %rdi,%rbx
  402036:       48 83 ec 10             sub    $0x10,%rsp
  40203a:       e8 a1 0b 00 00          call   402be0 <valid_u32_fields>
  40203f:       84 c0                   test   %al,%al
  402041:       75 0d                   jne    402050 <phase_01+0x30>
  402043:       48 83 c4 10             add    $0x10,%rsp
  402047:       31 c0                   xor    %eax,%eax
  402049:       5b                      pop    %rbx
  40204a:       41 5c                   pop    %r12
  40204c:       5d                      pop    %rbp
  40204d:       c3                      ret
  40204e:       66 90                   xchg   %ax,%ax
  402050:       31 c0                   xor    %eax,%eax
  402052:       48 8d 55 ec             lea    -0x14(%rbp),%rdx
  402056:       be 82 31 40 00          mov    $0x403182,%esi
  40205b:       48 89 df                mov    %rbx,%rdi
  40205e:       e8 7d f1 ff ff          call   4011e0 <__isoc99_sscanf@plt>
  402063:       83 f8 01                cmp    $0x1,%eax
  402066:       75 db                   jne    402043 <phase_01+0x23>
  402068:       4c 89 e7                mov    %r12,%rdi
  40206b:       e8 70 ff ff ff          call   401fe0 <p1_compute>
  402070:       39 45 ec                cmp    %eax,-0x14(%rbp)
  402073:       0f 94 c0                sete   %al
  402076:       48 83 c4 10             add    $0x10,%rsp
  40207a:       5b                      pop    %rbx
  40207b:       41 5c                   pop    %r12
  40207d:       5d                      pop    %rbp
  40207e:       c3                      ret
  40207f:       90                      nop
```
输入1个数，实际上是对比<p1_compute>函数的输出结果，我们反汇编该函数：
```
0000000000401fe0 <p1_compute>:
  401fe0:       f3 0f 1e fa             endbr64
  401fe4:       55                      push   %rbp
  401fe5:       48 89 fa                mov    %rdi,%rdx
  401fe8:       48 89 f8                mov    %rdi,%rax
  401feb:       48 c1 e8 20             shr    $0x20,%rax
  401fef:       48 89 d6                mov    %rdx,%rsi
  401ff2:       89 c7                   mov    %eax,%edi
  401ff4:       48 c1 ee 10             shr    $0x10,%rsi
  401ff8:       48 89 e5                mov    %rsp,%rbp
  401ffb:       e8 c0 ff ff ff          call   401fc0 <p1_step1>
  402000:       89 fe                   mov    %edi,%esi
  402002:       89 d7                   mov    %edx,%edi
  402004:       89 c1                   mov    %eax,%ecx
  402006:       e8 a5 ff ff ff          call   401fb0 <p1_step0>
  40200b:       89 ce                   mov    %ecx,%esi
  40200d:       89 c7                   mov    %eax,%edi
  40200f:       e8 bc ff ff ff          call   401fd0 <p1_step2>
  402014:       5d                      pop    %rbp
  402015:       c3                      ret
  402016:       66 2e 0f 1f 84 00 00    cs nopw 0x0(%rax,%rax,1)
  40201d:       00 00 00 
```
该函数先对输入的challenge做出位移运算，并存储在三个不同的寄存器当中：`%rdx = challenge`, `%rax = challenge >> 32`, `%rsi = challenge >> 16`

然后调用3个步骤函数，对这三个寄存器中的数分别进行计算，反汇编step1 0 2三个函数
```
0000000000401fb0 <p1_step0>:
  401fb0:       f3 0f 1e fa             endbr64
  401fb4:       8d 04 7f                lea    (%rdi,%rdi,2),%eax
  401fb7:       81 c6 71 d7 00 00       add    $0xd771,%esi
  401fbd:       31 f0                   xor    %esi,%eax
  401fbf:       c3                      ret

0000000000401fc0 <p1_step1>:
  401fc0:       f3 0f 1e fa             endbr64
  401fc4:       8d 04 f7                lea    (%rdi,%rsi,8),%eax
  401fc7:       35 55 91 00 00          xor    $0x9155,%eax
  401fcc:       c3                      ret
  401fcd:       0f 1f 00                nopl   (%rax)

0000000000401fd0 <p1_step2>:
  401fd0:       f3 0f 1e fa             endbr64
  401fd4:       81 f6 6e 12 00 00       xor    $0x126e,%esi
  401fda:       8d 04 fe                lea    (%rsi,%rdi,8),%eax
  401fdd:       c3                      ret
  401fde:       66 90                   xchg   %ax,%ax
```
用c翻译这三个函数，注意三个参数的调用顺序，并完成phase1，测试通过

## phase 2
反汇编phase02，得到如下编译码：
```
0000000000402080 <phase_02>:
  402080:       f3 0f 1e fa             endbr64
  402084:       55                      push   %rbp
  402085:       48 89 e5                mov    %rsp,%rbp
  402088:       41 55                   push   %r13
  40208a:       49 89 f5                mov    %rsi,%r13
  40208d:       be 06 00 00 00          mov    $0x6,%esi
  402092:       41 54                   push   %r12
  402094:       49 89 fc                mov    %rdi,%r12
  402097:       53                      push   %rbx
  402098:       48 83 ec 28             sub    $0x28,%rsp
  40209c:       e8 3f 0b 00 00          call   402be0 <valid_u32_fields>
  4020a1:       84 c0                   test   %al,%al
  4020a3:       75 13                   jne    4020b8 <phase_02+0x38>
  4020a5:       31 db                   xor    %ebx,%ebx
  4020a7:       48 8d 65 e8             lea    -0x18(%rbp),%rsp
  4020ab:       89 d8                   mov    %ebx,%eax
  4020ad:       5b                      pop    %rbx
  4020ae:       41 5c                   pop    %r12
  4020b0:       41 5d                   pop    %r13
  4020b2:       5d                      pop    %rbp
  4020b3:       c3                      ret
  4020b4:       0f 1f 40 00             nopl   0x0(%rax)
  4020b8:       89 c3                   mov    %eax,%ebx
  4020ba:       48 8d 45 d4             lea    -0x2c(%rbp),%rax
  4020be:       48 8d 4d c4             lea    -0x3c(%rbp),%rcx
  4020c2:       be 73 31 40 00          mov    $0x403173,%esi
  4020c7:       50                      push   %rax
  4020c8:       48 8d 45 d0             lea    -0x30(%rbp),%rax
  4020cc:       48 8d 55 c0             lea    -0x40(%rbp),%rdx
  4020d0:       4c 89 e7                mov    %r12,%rdi
  4020d3:       50                      push   %rax
  4020d4:       4c 8d 4d cc             lea    -0x34(%rbp),%r9
  4020d8:       4c 8d 45 c8             lea    -0x38(%rbp),%r8
  4020dc:       31 c0                   xor    %eax,%eax
  4020de:       e8 fd f0 ff ff          call   4011e0 <__isoc99_sscanf@plt>
  4020e3:       5a                      pop    %rdx
  4020e4:       59                      pop    %rcx
  4020e5:       83 f8 06                cmp    $0x6,%eax
  4020e8:       75 bb                   jne    4020a5 <phase_02+0x25>
  4020ea:       4c 89 e8                mov    %r13,%rax
  4020ed:       8b 75 c0                mov    -0x40(%rbp),%esi
  4020f0:       48 c1 e8 2d             shr    $0x2d,%rax
  4020f4:       01 c0                   add    %eax,%eax
  4020f6:       83 e0 7e                and    $0x7e,%eax
  4020f9:       83 c0 13                add    $0x13,%eax
  4020fc:       39 c6                   cmp    %eax,%esi
  4020fe:       75 a5                   jne    4020a5 <phase_02+0x25>
  402100:       b9 0c 00 00 00          mov    $0xc,%ecx
  402105:       ba 01 00 00 00          mov    $0x1,%edx
  40210a:       4c 89 e8                mov    %r13,%rax
  40210d:       48 d3 e8                shr    %cl,%rax
  402110:       83 e0 1f                and    $0x1f,%eax
  402113:       01 f0                   add    %esi,%eax
  402115:       8d 34 d5 00 00 00 00    lea    0x0(,%rdx,8),%esi
  40211c:       31 f0                   xor    %esi,%eax
  40211e:       8b 74 95 c0             mov    -0x40(%rbp,%rdx,4),%esi
  402122:       39 c6                   cmp    %eax,%esi
  402124:       0f 85 7b ff ff ff       jne    4020a5 <phase_02+0x25>
  40212a:       48 83 c2 01             add    $0x1,%rdx
  40212e:       83 c1 05                add    $0x5,%ecx
  402131:       48 83 fa 06             cmp    $0x6,%rdx
  402135:       75 d3                   jne    40210a <phase_02+0x8a>
  402137:       e9 6b ff ff ff          jmp    4020a7 <phase_02+0x27>
  40213c:       0f 1f 40 00             nopl   0x0(%rax)
```
count应该为6个数。
第一个整数的计算为 `((first * 2) & 0x7e) + 0x13`。在主函数入口显示的编译码当中观察到后面有类似的for循环。先找到循环初始化
```
mov    $0xc,%ecx
mov    $0x1,%edx
```
然后观察循环主体：
```
mov    %r13,%rax
shr    %cl,%rax
and    $0x1f,%eax
add    %esi,%eax
```
在这里我们分别看调用的三个寄存器：%r13寄存器是 challenge；
%cl保存的是右移位数；
%esi保存了前一个整数。然后就可以写出c代码了

## phase 3
反汇编phase03，得到如下编译码：
```
0000000000402220 <phase_03>:
  402220:       f3 0f 1e fa             endbr64
  402224:       55                      push   %rbp
  402225:       48 89 e5                mov    %rsp,%rbp
  402228:       41 54                   push   %r12
  40222a:       49 89 f4                mov    %rsi,%r12
  40222d:       be 02 00 00 00          mov    $0x2,%esi
  402232:       53                      push   %rbx
  402233:       48 89 fb                mov    %rdi,%rbx
  402236:       48 83 ec 10             sub    $0x10,%rsp
  40223a:       e8 a1 09 00 00          call   402be0 <valid_u32_fields>
  40223f:       84 c0                   test   %al,%al
  402241:       75 0d                   jne    402250 <phase_03+0x30>
  402243:       31 c0                   xor    %eax,%eax
  402245:       48 83 c4 10             add    $0x10,%rsp
  402249:       5b                      pop    %rbx
  40224a:       41 5c                   pop    %r12
  40224c:       5d                      pop    %rbp
  40224d:       c3                      ret
  40224e:       66 90                   xchg   %ax,%ax
  402250:       31 c0                   xor    %eax,%eax
  402252:       48 8d 4d ec             lea    -0x14(%rbp),%rcx
  402256:       48 8d 55 e8             lea    -0x18(%rbp),%rdx
  40225a:       be 7f 31 40 00          mov    $0x40317f,%esi
  40225f:       48 89 df                mov    %rbx,%rdi
  402262:       e8 79 ef ff ff          call   4011e0 <__isoc99_sscanf@plt>
  402267:       83 f8 02                cmp    $0x2,%eax
  40226a:       75 d7                   jne    402243 <phase_03+0x23>
  40226c:       44 89 e7                mov    %r12d,%edi
  40226f:       31 c0                   xor    %eax,%eax
  402271:       83 e7 07                and    $0x7,%edi
  402274:       39 7d e8                cmp    %edi,-0x18(%rbp)
  402277:       75 cc                   jne    402245 <phase_03+0x25>
  402279:       4c 89 e0                mov    %r12,%rax
  40227c:       4c 89 e6                mov    %r12,%rsi
  40227f:       48 c1 e8 03             shr    $0x3,%rax
  402283:       48 c1 ee 20             shr    $0x20,%rsi
  402287:       31 c6                   xor    %eax,%esi
  402289:       e8 32 ff ff ff          call   4021c0 <p3_dispatch>
  40228e:       39 45 ec                cmp    %eax,-0x14(%rbp)
  402291:       0f 94 c0                sete   %al
  402294:       eb af                   jmp    402245 <phase_03+0x25>
  402296:       66 2e 0f 1f 84 00 00    cs nopw 0x0(%rax,%rax,1)
  40229d:       00 00 00 
```
观察到一共输入两个整数，其中第一个数的运算为：
```
mov    %r12d,%edi
and    $0x7,%edi
cmp    %edi,-0x18(%rbp)
```
即`first = (uint32_t)(challenge & 0x7);`
```
mov    %r12,%rax
mov    %r12,%rsi
shr    $0x3,%rax
shr    $0x20,%rsi
xor    %eax,%esi
call   4021c0 <p3_dispatch>
```
处理第二个整数是位移后异或，`key = (challenge >> 3) ^ (challenge >> 31)`。
之后再用p3_dispatch函数调用first和key。该函数反汇编如下：
```
00000000004021c0 <p3_dispatch>:
  4021c0:       f3 0f 1e fa             endbr64
  4021c4:       89 f8                   mov    %edi,%eax
  4021c6:       89 f7                   mov    %esi,%edi
  4021c8:       83 f8 07                cmp    $0x7,%eax
  4021cb:       77 48                   ja     402215 <p3_dispatch+0x55>
  4021cd:       3e ff 24 c5 e0 57 40    notrack jmp *0x4057e0(,%rax,8)
  4021d4:       00 
  4021d5:       0f 1f 00                nopl   (%rax)
  4021d8:       eb d6                   jmp    4021b0 <p3_case_7>
  4021da:       66 0f 1f 44 00 00       nopw   0x0(%rax,%rax,1)
  4021e0:       e9 7b ff ff ff          jmp    402160 <p3_case_2>
  4021e5:       0f 1f 00                nopl   (%rax)
  4021e8:       eb a6                   jmp    402190 <p3_case_5>
  4021ea:       66 0f 1f 44 00 00       nopw   0x0(%rax,%rax,1)
  4021f0:       eb 8e                   jmp    402180 <p3_case_4>
  4021f2:       66 0f 1f 44 00 00       nopw   0x0(%rax,%rax,1)
  4021f8:       e9 43 ff ff ff          jmp    402140 <p3_case_0>
  4021fd:       0f 1f 00                nopl   (%rax)
  402200:       eb 9e                   jmp    4021a0 <p3_case_6>
  402202:       66 0f 1f 44 00 00       nopw   0x0(%rax,%rax,1)
  402208:       e9 63 ff ff ff          jmp    402170 <p3_case_3>
  40220d:       0f 1f 00                nopl   (%rax)
  402210:       e9 3b ff ff ff          jmp    402150 <p3_case_1>
  402215:       31 c0                   xor    %eax,%eax
  402217:       c3                      ret
  402218:       0f 1f 84 00 00 00 00    nopl   0x0(%rax,%rax,1)
  40221f:       00 
```
`notrack jmp *0x4057e0(,%rax,8)`是典型switch函数的跳转表，为了翻译出这个switch函数，我们查看7个case的汇编码：
```
0000000000402140 <p3_case_0>:
  402140:       f3 0f 1e fa             endbr64
  402144:       89 f8                   mov    %edi,%eax
  402146:       d1 ef                   shr    $1,%edi
  402148:       35 c2 70 00 00          xor    $0x70c2,%eax
  40214d:       01 f8                   add    %edi,%eax
  40214f:       c3                      ret

0000000000402150 <p3_case_1>:
  402150:       f3 0f 1e fa             endbr64
  402154:       8d 84 bf c8 91 00 00    lea    0x91c8(%rdi,%rdi,4),%eax
  40215b:       c3                      ret
  40215c:       0f 1f 40 00             nopl   0x0(%rax)

0000000000402160 <p3_case_2>:
  402160:       f3 0f 1e fa             endbr64
  402164:       8d 04 3f                lea    (%rdi,%rdi,1),%eax
  402167:       35 0a 33 00 00          xor    $0x330a,%eax
  40216c:       c3                      ret
  40216d:       0f 1f 00                nopl   (%rax)

0000000000402170 <p3_case_3>:
  402170:       f3 0f 1e fa             endbr64
  402174:       8d 84 bf de 02 01 00    lea    0x102de(%rdi,%rdi,4),%eax
  40217b:       c3                      ret
  40217c:       0f 1f 40 00             nopl   0x0(%rax)

0000000000402180 <p3_case_4>:
  402180:       f3 0f 1e fa             endbr64
  402184:       8d 84 7f 84 ce 00 00    lea    0xce84(%rdi,%rdi,2),%eax
  40218b:       c3                      ret
  40218c:       0f 1f 40 00             nopl   0x0(%rax)

0000000000402190 <p3_case_5>:
  402190:       f3 0f 1e fa             endbr64
  402194:       8d 04 3f                lea    (%rdi,%rdi,1),%eax
  402197:       35 50 e3 00 00          xor    $0xe350,%eax
  40219c:       c3                      ret
  40219d:       0f 1f 00                nopl   (%rax)

00000000004021a0 <p3_case_6>:
  4021a0:       f3 0f 1e fa             endbr64
  4021a4:       89 f8                   mov    %edi,%eax
  4021a6:       d1 ef                   shr    $1,%edi
  4021a8:       35 9b 95 00 00          xor    $0x959b,%eax
  4021ad:       01 f8                   add    %edi,%eax
  4021af:       c3                      ret

00000000004021b0 <p3_case_7>:
  4021b0:       f3 0f 1e fa             endbr64
  4021b4:       8d 84 bf 57 93 01 00    lea    0x19357(%rdi,%rdi,4),%eax
  4021bb:       c3                      ret
  4021bc:       0f 1f 40 00             nopl   0x0(%rax)
```
都是简单的计算，直接翻译就可以得到答案。但是没有通过测试！！！

问题：switch的case表格位置是打乱的！不是0-7顺序，我们需要通过gdb动态测试，看出一个case后面的地址，找出正确的顺序。最后写出相应的c代码

## phase 4
反汇编phase04，得到如下编译码：
```
0000000000402310 <phase_04>:
  402310:       f3 0f 1e fa             endbr64
  402314:       55                      push   %rbp
  402315:       48 89 e5                mov    %rsp,%rbp
  402318:       41 54                   push   %r12
  40231a:       49 89 f4                mov    %rsi,%r12
  40231d:       be 01 00 00 00          mov    $0x1,%esi
  402322:       53                      push   %rbx
  402323:       48 89 fb                mov    %rdi,%rbx
  402326:       48 83 ec 10             sub    $0x10,%rsp
  40232a:       e8 b1 08 00 00          call   402be0 <valid_u32_fields>
  40232f:       84 c0                   test   %al,%al
  402331:       75 0d                   jne    402340 <phase_04+0x30>
  402333:       48 83 c4 10             add    $0x10,%rsp
  402337:       31 c0                   xor    %eax,%eax
  402339:       5b                      pop    %rbx
  40233a:       41 5c                   pop    %r12
  40233c:       5d                      pop    %rbp
  40233d:       c3                      ret
  40233e:       66 90                   xchg   %ax,%ax
  402340:       31 c0                   xor    %eax,%eax
  402342:       48 8d 55 ec             lea    -0x14(%rbp),%rdx
  402346:       be 82 31 40 00          mov    $0x403182,%esi
  40234b:       48 89 df                mov    %rbx,%rdi
  40234e:       e8 8d ee ff ff          call   4011e0 <__isoc99_sscanf@plt>
  402353:       83 f8 01                cmp    $0x1,%eax
  402356:       75 db                   jne    402333 <phase_04+0x23>
  402358:       4c 89 e1                mov    %r12,%rcx
  40235b:       4c 89 e0                mov    %r12,%rax
  40235e:       44 89 e7                mov    %r12d,%edi
  402361:       31 f6                   xor    %esi,%esi
  402363:       48 c1 e8 20             shr    $0x20,%rax
  402367:       48 c1 e9 07             shr    $0x7,%rcx
  40236b:       83 e7 7f                and    $0x7f,%edi
  40236e:       ba 7f 00 00 00          mov    $0x7f,%edx
  402373:       31 c1                   xor    %eax,%ecx
  402375:       e8 26 ff ff ff          call   4022a0 <p4_walk>
  40237a:       39 45 ec                cmp    %eax,-0x14(%rbp)
  40237d:       0f 94 c0                sete   %al
  402380:       48 83 c4 10             add    $0x10,%rsp
  402384:       5b                      pop    %rbx
  402385:       41 5c                   pop    %r12
  402387:       5d                      pop    %rbp
  402388:       c3                      ret
  402389:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)
```
只需要输入一个数。通过以下段落，对challenge做出处理：
```
mov    %r12,%rcx
mov    %r12,%rax
mov    %r12d,%edi
xor    %esi,%esi
shr    $0x20,%rax
shr    $0x7,%rcx
and    $0x7f,%edi
mov    $0x7f,%edx
xor    %eax,%ecx
call   p4_walk
```
这个之前已经做了很多类似的简单计算了，得到4个参数，传入函数p4_walk。该函数的汇编码如下：
```
00000000004022a0 <p4_walk>:
  4022a0:       f3 0f 1e fa             endbr64
  4022a4:       89 f8                   mov    %edi,%eax
  4022a6:       89 d7                   mov    %edx,%edi
  4022a8:       29 f7                   sub    %esi,%edi
  4022aa:       d1 ef                   shr    $1,%edi
  4022ac:       01 f7                   add    %esi,%edi
  4022ae:       39 c7                   cmp    %eax,%edi
  4022b0:       74 4e                   je     402300 <p4_walk+0x60>
  4022b2:       55                      push   %rbp
  4022b3:       48 89 e5                mov    %rsp,%rbp
  4022b6:       53                      push   %rbx
  4022b7:       8d 1c 0f                lea    (%rdi,%rcx,1),%ebx
  4022ba:       48 83 ec 08             sub    $0x8,%rsp
  4022be:       39 f8                   cmp    %edi,%eax
  4022c0:       72 1e                   jb     4022e0 <p4_walk+0x40>
  4022c2:       8d 77 01                lea    0x1(%rdi),%esi
  4022c5:       89 c7                   mov    %eax,%edi
  4022c7:       e8 d4 ff ff ff          call   4022a0 <p4_walk>
  4022cc:       8d 04 80                lea    (%rax,%rax,4),%eax
  4022cf:       31 d8                   xor    %ebx,%eax
  4022d1:       48 8b 5d f8             mov    -0x8(%rbp),%rbx
  4022d5:       c9                      leave
  4022d6:       83 f0 0c                xor    $0xc,%eax
  4022d9:       c3                      ret
  4022da:       66 0f 1f 44 00 00       nopw   0x0(%rax,%rax,1)
  4022e0:       8d 57 ff                lea    -0x1(%rdi),%edx
  4022e3:       89 c7                   mov    %eax,%edi
  4022e5:       e8 b6 ff ff ff          call   4022a0 <p4_walk>
  4022ea:       8d 04 80                lea    (%rax,%rax,4),%eax
  4022ed:       31 d8                   xor    %ebx,%eax
  4022ef:       48 8b 5d f8             mov    -0x8(%rbp),%rbx
  4022f3:       c9                      leave
  4022f4:       83 f0 02                xor    $0x2,%eax
  4022f7:       c3                      ret
  4022f8:       0f 1f 84 00 00 00 00    nopl   0x0(%rax,%rax,1)
  4022ff:       00 
  402300:       8d 47 75                lea    0x75(%rdi),%eax
  402303:       31 c8                   xor    %ecx,%eax
  402305:       c3                      ret
  402306:       66 2e 0f 1f 84 00 00    cs nopw 0x0(%rax,%rax,1)
  40230d:       00 00 00 
```
函数前半部分计算中点，仍然是熟悉的运算。观察到该函数出现了递归运算，先找到递归运算的终止：
```
lea    0x75(%rdi),%eax
xor    %ecx,%eax
ret
```
即`return (mid + 0x75) ^ key`;后面再找到递归有两种情况，一个是值比中点大，一个是比中点小，分别找到return的值。

## phase 5
反汇编phase05，得到如下编译码：
```
00000000004023f0 <phase_05>:
  4023f0:       f3 0f 1e fa             endbr64
  4023f4:       55                      push   %rbp
  4023f5:       48 89 e5                mov    %rsp,%rbp
  4023f8:       41 54                   push   %r12
  4023fa:       49 89 f4                mov    %rsi,%r12
  4023fd:       be 02 00 00 00          mov    $0x2,%esi
  402402:       53                      push   %rbx
  402403:       48 89 fb                mov    %rdi,%rbx
  402406:       48 83 ec 10             sub    $0x10,%rsp
  40240a:       e8 d1 07 00 00          call   402be0 <valid_u32_fields>
  40240f:       84 c0                   test   %al,%al
  402411:       75 0d                   jne    402420 <phase_05+0x30>
  402413:       31 c0                   xor    %eax,%eax
  402415:       48 83 c4 10             add    $0x10,%rsp
  402419:       5b                      pop    %rbx
  40241a:       41 5c                   pop    %r12
  40241c:       5d                      pop    %rbp
  40241d:       c3                      ret
  40241e:       66 90                   xchg   %ax,%ax
  402420:       31 c0                   xor    %eax,%eax
  402422:       48 8d 4d e8             lea    -0x18(%rbp),%rcx
  402426:       48 8d 55 e4             lea    -0x1c(%rbp),%rdx
  40242a:       be 7f 31 40 00          mov    $0x40317f,%esi
  40242f:       48 89 df                mov    %rbx,%rdi
  402432:       e8 a9 ed ff ff          call   4011e0 <__isoc99_sscanf@plt>
  402437:       83 f8 02                cmp    $0x2,%eax
  40243a:       75 d7                   jne    402413 <phase_05+0x23>
  40243c:       48 8d 75 ec             lea    -0x14(%rbp),%rsi
  402440:       4c 89 e7                mov    %r12,%rdi
  402443:       e8 48 ff ff ff          call   402390 <p5_traverse>
  402448:       89 c2                   mov    %eax,%edx
  40244a:       31 c0                   xor    %eax,%eax
  40244c:       39 55 e4                cmp    %edx,-0x1c(%rbp)
  40244f:       75 c4                   jne    402415 <phase_05+0x25>
  402451:       8b 45 ec                mov    -0x14(%rbp),%eax
  402454:       39 45 e8                cmp    %eax,-0x18(%rbp)
  402457:       0f 94 c0                sete   %al
  40245a:       eb b9                   jmp    402415 <phase_05+0x25>
  40245c:       0f 1f 40 00             nopl   0x0(%rax)
```
需要输入两个参数。
```
lea    -0x14(%rbp),%rsi
mov    %r12,%rdi
call   402390 <p5_traverse>
```
注意到调用函数p5_traverse的时候，一个是challenge，还有一个是一个地址。先看调用完函数的内容。
```
mov    %eax,%edx
cmp    %edx,-0x1c(%rbp)
```
验证第一个值和p5_traverse的值是否相等

```
mov    -0x14(%rbp),%eax
cmp    %eax,-0x18(%rbp)
```
验证第二个值和p5_traverse写入内存的值是否相等，然后我们再具体看该函数的具体汇编码：
```
0000000000402390 <p5_traverse>:
  402390:       f3 0f 1e fa             endbr64
  402394:       48 89 f8                mov    %rdi,%rax
  402397:       48 89 f9                mov    %rdi,%rcx
  40239a:       49 89 f0                mov    %rsi,%r8
  40239d:       83 e0 07                and    $0x7,%eax
  4023a0:       48 c1 e9 03             shr    $0x3,%rcx
  4023a4:       48 8b 14 c5 20 5b 40    mov    0x405b20(,%rax,8),%rdx
  4023ab:       00 
  4023ac:       48 89 f8                mov    %rdi,%rax
  4023af:       48 c1 ef 20             shr    $0x20,%rdi
  4023b3:       48 c1 e8 06             shr    $0x6,%rax
  4023b7:       31 f8                   xor    %edi,%eax
  4023b9:       8b 72 04                mov    0x4(%rdx),%esi
  4023bc:       89 cf                   mov    %ecx,%edi
  4023be:       35 a6 38 00 00          xor    $0x38a6,%eax
  4023c3:       8d 04 40                lea    (%rax,%rax,2),%eax
  4023c6:       03 02                   add    (%rdx),%eax
  4023c8:       41 89 30                mov    %esi,(%r8)
  4023cb:       31 f0                   xor    %esi,%eax
  4023cd:       83 e7 03                and    $0x3,%edi
  4023d0:       74 1d                   je     4023ef <p5_traverse+0x5f>
  4023d2:       b9 01 00 00 00          mov    $0x1,%ecx
  4023d7:       48 8b 52 08             mov    0x8(%rdx),%rdx
  4023db:       8d 04 40                lea    (%rax,%rax,2),%eax
  4023de:       83 c1 01                add    $0x1,%ecx
  4023e1:       8b 72 04                mov    0x4(%rdx),%esi
  4023e4:       03 02                   add    (%rdx),%eax
  4023e6:       31 f0                   xor    %esi,%eax
  4023e8:       41 89 30                mov    %esi,(%r8)
  4023eb:       39 cf                   cmp    %ecx,%edi
  4023ed:       73 e8                   jae    4023d7 <p5_traverse+0x47>
  4023ef:       c3                      ret
```
我们先分析出这其实是一个链表一样的遍历
```
mov    %rdi,%rax
and    $0x7,%eax
mov    0x405b20(,%rax,8),%rdx
```
这段运算存储在%rdx里的是调用节点的个数
```
mov    %rdi,%rcx
shr    $0x3,%rcx
mov    %ecx,%edi
and    $0x3,%edi
```
这里利用challenge决定遍历次数，注意到次数在0-3。然后我们能找到每个节点的简单计算，就不单独写出了。但是为了知道节点具体内容，需要用gdb动态调试。
```
(gdb) x/8gx 0x405b20
0x405b20 <p5_heads>:    0x0000000000405b90      0x0000000000405b80
0x405b30 <p5_heads+16>: 0x0000000000405bc0      0x0000000000405c00
0x405b40 <p5_heads+32>: 0x0000000000405b70      0x0000000000405bd0
0x405b50 <p5_heads+48>: 0x0000000000405bb0      0x0000000000405bf0
```
找到八个节点的头地址，然后再分别查看每个节点的内容，以下是第一个节点的示例
```
(gdb) x/2wx 0x405b90
0x405b90 <p5_nodes+48>: 0x00000836      0x00000a31
(gdb) x/gx 0x405b98
0x405b98 <p5_nodes+56>: 0x0000000000405b80
```
以上三个数据分别是两个值和next地址，用c语言来描述大概是一个链表节点：
```
struct Node {
    uint32_t value1;
    uint32_t value2;
    struct Node *next;
};
```
我们读取全部8个节点后，注意到第八个节点仍然有next指针，
```
(gdb) x/2wx 0x405c10
0x405c10 <p5_nodes+176>:        0x00006463      0x0000052f
(gdb) x/gx 0x405c18
0x405c18 <p5_nodes+184>:        0x0000000000405b60
```
第九个节点仍然有下一个节点指针。因为最多遍历3个节点，所以仍然需要再看一个,一共读到第十个接节点

最终即可写出最终的c代码。
## phase 6
反汇编phase06，得到如下编译码：
```
0000000000402530 <phase_06>:
  402530:       f3 0f 1e fa             endbr64
  402534:       55                      push   %rbp
  402535:       48 89 e5                mov    %rsp,%rbp
  402538:       41 54                   push   %r12
  40253a:       49 89 f4                mov    %rsi,%r12
  40253d:       be 01 00 00 00          mov    $0x1,%esi
  402542:       53                      push   %rbx
  402543:       48 89 fb                mov    %rdi,%rbx
  402546:       48 83 ec 10             sub    $0x10,%rsp
  40254a:       e8 91 06 00 00          call   402be0 <valid_u32_fields>
  40254f:       84 c0                   test   %al,%al
  402551:       75 0d                   jne    402560 <phase_06+0x30>
  402553:       48 83 c4 10             add    $0x10,%rsp
  402557:       31 c0                   xor    %eax,%eax
  402559:       5b                      pop    %rbx
  40255a:       41 5c                   pop    %r12
  40255c:       5d                      pop    %rbp
  40255d:       c3                      ret
  40255e:       66 90                   xchg   %ax,%ax
  402560:       31 c0                   xor    %eax,%eax
  402562:       48 8d 55 ec             lea    -0x14(%rbp),%rdx
  402566:       be 82 31 40 00          mov    $0x403182,%esi
  40256b:       48 89 df                mov    %rbx,%rdi
  40256e:       e8 6d ec ff ff          call   4011e0 <__isoc99_sscanf@plt>
  402573:       83 f8 01                cmp    $0x1,%eax
  402576:       75 db                   jne    402553 <phase_06+0x23>
  402578:       4c 89 e7                mov    %r12,%rdi
  40257b:       e8 60 ff ff ff          call   4024e0 <p6_dispatch>
  402580:       39 45 ec                cmp    %eax,-0x14(%rbp)
  402583:       0f 94 c0                sete   %al
  402586:       48 83 c4 10             add    $0x10,%rsp
  40258a:       5b                      pop    %rbx
  40258b:       41 5c                   pop    %r12
  40258d:       5d                      pop    %rbp
  40258e:       c3                      ret
  40258f:       90                      nop
```
```
mov    %r12,%rdi
call   4024e0 <p6_dispatch>
cmp    %eax,-0x14(%rbp)
sete   %al
```
输入一个数，判断该数是否等于调用函数p6_dispatch的返回值，直接看该函数的编译码：
```
00000000004024e0 <p6_dispatch>:
  4024e0:       f3 0f 1e fa             endbr64
  4024e4:       55                      push   %rbp
  4024e5:       48 89 e5                mov    %rsp,%rbp
  4024e8:       53                      push   %rbx
  4024e9:       48 89 fb                mov    %rdi,%rbx
  4024ec:       48 c1 ef 06             shr    $0x6,%rdi
  4024f0:       48 89 d8                mov    %rbx,%rax
  4024f3:       48 c1 e8 20             shr    $0x20,%rax
  4024f7:       31 c7                   xor    %eax,%edi
  4024f9:       48 89 d8                mov    %rbx,%rax
  4024fc:       48 83 ec 08             sub    $0x8,%rsp
  402500:       83 e3 38                and    $0x38,%ebx
  402503:       83 e0 07                and    $0x7,%eax
  402506:       ff 14 c5 c0 5d 40 00    call   *0x405dc0(,%rax,8)
  40250d:       35 1d b2 00 00          xor    $0xb21d,%eax
  402512:       89 c7                   mov    %eax,%edi
  402514:       ff 93 c0 5d 40 00       call   *0x405dc0(%rbx)
  40251a:       48 8b 5d f8             mov    -0x8(%rbp),%rbx
  40251e:       c9                      leave
  40251f:       05 14 c8 00 00          add    $0xc814,%eax
  402524:       c3                      ret
  402525:       66 66 2e 0f 1f 84 00    data16 cs nopw 0x0(%rax,%rax,1)
  40252c:       00 00 00 00 
```
该函数连续调用了两次函数表，先看第一次调用：
```
mov    %rbx,%rax
shr    $0x6,%rdi
shr    $0x20,%rax
xor    %eax,%edi
and    $0x7,%eax
call   *0x405dc0(,%rax,8)
```
%rax当中存储了第一次的索引值，结果存储在%eax当中，同时注意到该函数表当中一共有8个函数。再来看第二次调用：
```
xor    $0xb21d,%eax
mov    %eax,%edi
call   *0x405dc0(%rbx)
```
第二次调用则用了第一次的结果，现在知道了参数的关系，先确定他们的名字，
```
0000000000402460 T p6_op_0
0000000000402470 T p6_op_1
0000000000402480 T p6_op_2
0000000000402490 T p6_op_3
00000000004024b0 T p6_op_4
00000000004024d0 T p6_op_5
00000000004024e0 T p6_dispatch
0000000000402530 T phase_06
0000000000405dc0 r p6_ops
0000000000405e00 r p6_agent_urgent
```
然后用gdb找出他们,
```
(gdb) x/8gx 0x405dc0
0x405dc0 <p6_ops>:      0x0000000000402460      0x0000000000402470
0x405dd0 <p6_ops+16>:   0x00000000004024d0      0x00000000004024d0
0x405de0 <p6_ops+32>:   0x00000000004024b0      0x0000000000402480
0x405df0 <p6_ops+48>:   0x0000000000402490      0x0000000000402480
```
反汇编这几个函数即可，都是很简单的计算，我们直接用switch函数即可

## secret
TODO指示中明确有三个函数，逐个反汇编：
```
0000000000402690 <secret_gate>:
  402690:       f3 0f 1e fa             endbr64
  402694:       55                      push   %rbp
  402695:       31 c0                   xor    %eax,%eax
  402697:       b9 14 00 00 00          mov    $0x14,%ecx
  40269c:       48 89 e5                mov    %rsp,%rbp
  40269f:       41 56                   push   %r14
  4026a1:       49 89 d6                mov    %rdx,%r14
  4026a4:       41 55                   push   %r13
  4026a6:       49 89 f5                mov    %rsi,%r13
  4026a9:       41 54                   push   %r12
  4026ab:       4c 8d a5 30 ff ff ff    lea    -0xd0(%rbp),%r12
  4026b2:       53                      push   %rbx
  4026b3:       48 89 fb                mov    %rdi,%rbx
  4026b6:       4c 89 e7                mov    %r12,%rdi
  4026b9:       48 81 ec b0 00 00 00    sub    $0xb0,%rsp
  4026c0:       f3 48 ab                rep stos %rax,(%rdi)
  4026c3:       c7 07 00 00 00 00       movl   $0x0,(%rdi)
  4026c9:       48 89 df                mov    %rbx,%rdi
  4026cc:       e8 bf ea ff ff          call   401190 <strlen@plt>
  4026d1:       48 83 e8 24             sub    $0x24,%rax
  4026d5:       48 83 f8 7f             cmp    $0x7f,%rax
  4026d9:       76 15                   jbe    4026f0 <secret_gate+0x60>
  4026db:       48 81 c4 b0 00 00 00    add    $0xb0,%rsp
  4026e2:       31 c0                   xor    %eax,%eax
  4026e4:       5b                      pop    %rbx
  4026e5:       41 5c                   pop    %r12
  4026e7:       41 5d                   pop    %r13
  4026e9:       41 5e                   pop    %r14
  4026eb:       5d                      pop    %rbp
  4026ec:       c3                      ret
  4026ed:       0f 1f 00                nopl   (%rax)
  4026f0:       48 89 de                mov    %rbx,%rsi
  4026f3:       4c 89 e7                mov    %r12,%rdi
  4026f6:       e8 75 ff ff ff          call   402670 <secret_copy>
  4026fb:       81 bd 50 ff ff ff 4f    cmpl   $0x4e45504f,-0xb0(%rbp)
  402702:       50 45 4e 
  402705:       75 d4                   jne    4026db <secret_gate+0x4b>
  402707:       4c 89 f2                mov    %r14,%rdx
  40270a:       4c 89 ee                mov    %r13,%rsi
  40270d:       48 8d bd 54 ff ff ff    lea    -0xac(%rbp),%rdi
  402714:       e8 97 fe ff ff          call   4025b0 <secret_phase>
  402719:       48 81 c4 b0 00 00 00    add    $0xb0,%rsp
  402720:       5b                      pop    %rbx
  402721:       41 5c                   pop    %r12
  402723:       41 5d                   pop    %r13
  402725:       41 5e                   pop    %r14
  402727:       5d                      pop    %rbp
  402728:       c3                      ret
  402729:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)
  ```
这其实不是关键的部分，而是一个入口（所以叫gate嘛）
```
call   strlen@plt
sub    $0x24,%rax
cmp    $0x7f,%rax
jbe    4026f0
```
显然，输入的字符串长度范围为36-127
```
call   secret_copy
cmpl   $0x4e45504f,-0xb0(%rbp)
```
secret-copy被调用后，需要和一串数字做对比，把二进制转换为字母，刚好就是OPEN，也就是说，在调用完该函数后，字符串必须要以OPEN开头，我们再来看具体实现
```
0000000000402670 <secret_copy>:
  402670:       f3 0f 1e fa             endbr64
  402674:       31 c0                   xor    %eax,%eax
  402676:       66 2e 0f 1f 84 00 00    cs nopw 0x0(%rax,%rax,1)
  40267d:       00 00 00 
  402680:       0f b6 14 06             movzbl (%rsi,%rax,1),%edx
  402684:       88 14 07                mov    %dl,(%rdi,%rax,1)
  402687:       48 83 c0 01             add    $0x1,%rax
  40268b:       84 d2                   test   %dl,%dl
  40268d:       75 f1                   jne    402680 <secret_copy+0x10>
  40268f:       c3                      ret
```
很明显，这就是去掉OPEN然后把数据原样传送给secret_phase，我们直接看该函数的编译码：
```
00000000004025b0 <secret_phase>:
  4025b0:       f3 0f 1e fa             endbr64
  4025b4:       55                      push   %rbp
  4025b5:       48 89 e5                mov    %rsp,%rbp
  4025b8:       41 55                   push   %r13
  4025ba:       49 89 d5                mov    %rdx,%r13
  4025bd:       41 54                   push   %r12
  4025bf:       49 89 f4                mov    %rsi,%r12
  4025c2:       be 03 00 00 00          mov    $0x3,%esi
  4025c7:       53                      push   %rbx
  4025c8:       48 89 fb                mov    %rdi,%rbx
  4025cb:       48 83 ec 18             sub    $0x18,%rsp
  4025cf:       e8 0c 06 00 00          call   402be0 <valid_u32_fields>
  4025d4:       84 c0                   test   %al,%al
  4025d6:       75 18                   jne    4025f0 <secret_phase+0x40>
  4025d8:       31 d2                   xor    %edx,%edx
  4025da:       48 83 c4 18             add    $0x18,%rsp
  4025de:       89 d0                   mov    %edx,%eax
  4025e0:       5b                      pop    %rbx
  4025e1:       41 5c                   pop    %r12
  4025e3:       41 5d                   pop    %r13
  4025e5:       5d                      pop    %rbp
  4025e6:       c3                      ret
  4025e7:       66 0f 1f 84 00 00 00    nopw   0x0(%rax,%rax,1)
  4025ee:       00 00 
  4025f0:       31 c0                   xor    %eax,%eax
  4025f2:       4c 8d 45 dc             lea    -0x24(%rbp),%r8
  4025f6:       48 8d 4d d8             lea    -0x28(%rbp),%rcx
  4025fa:       be 7c 31 40 00          mov    $0x40317c,%esi
  4025ff:       48 8d 55 d4             lea    -0x2c(%rbp),%rdx
  402603:       48 89 df                mov    %rbx,%rdi
  402606:       e8 d5 eb ff ff          call   4011e0 <__isoc99_sscanf@plt>
  40260b:       83 f8 03                cmp    $0x3,%eax
  40260e:       75 c8                   jne    4025d8 <secret_phase+0x28>
  402610:       44 89 e9                mov    %r13d,%ecx
  402613:       31 d2                   xor    %edx,%edx
  402615:       83 e1 03                and    $0x3,%ecx
  402618:       39 4d d4                cmp    %ecx,-0x2c(%rbp)
  40261b:       75 bd                   jne    4025da <secret_phase+0x2a>
  40261d:       4c 89 e6                mov    %r12,%rsi
  402620:       44 69 c9 01 ca 00 00    imul   $0xca01,%ecx,%r9d
  402627:       4d 89 e8                mov    %r13,%r8
  40262a:       48 c1 ee 20             shr    $0x20,%rsi
  40262e:       49 c1 e8 20             shr    $0x20,%r8
  402632:       44 31 e6                xor    %r12d,%esi
  402635:       45 31 e8                xor    %r13d,%r8d
  402638:       49 c1 ed 10             shr    $0x10,%r13
  40263c:       89 f7                   mov    %esi,%edi
  40263e:       45 31 e9                xor    %r13d,%r9d
  402641:       44 31 c7                xor    %r8d,%edi
  402644:       e8 47 ff ff ff          call   402590 <secret_mix>
  402649:       44 31 c8                xor    %r9d,%eax
  40264c:       39 45 d8                cmp    %eax,-0x28(%rbp)
  40264f:       75 89                   jne    4025da <secret_phase+0x2a>
  402651:       c1 ee 03                shr    $0x3,%esi
  402654:       41 8d 94 08 a2 08 00    lea    0x8a2(%r8,%rcx,1),%edx
  40265b:       00 
  40265c:       01 f2                   add    %esi,%edx
  40265e:       31 d0                   xor    %edx,%eax
  402660:       3b 45 dc                cmp    -0x24(%rbp),%eax
  402663:       0f 94 c2                sete   %dl
  402666:       e9 6f ff ff ff          jmp    4025da <secret_phase+0x2a>
  40266b:       0f 1f 44 00 00          nopl   0x0(%rax,%rax,1)

```
首先输入是三个数，我们分别来看：
```
mov %r13d,%ecx
and $0x3,%ecx
cmp %ecx,-0x2c(%rbp)
```
第一个数显然保留了context的后两位。第二个整数进行了一些我们熟悉的简单异或，位移等计算，得到一个新参数传入函数secret_mix,第三个数是在之前的结果上做简单计算，我们直接看secret_mix：
```
0000000000402590 <secret_mix>:
  402590:       f3 0f 1e fa             endbr64
  402594:       89 f8                   mov    %edi,%eax
  402596:       81 f7 01 ca 00 00       xor    $0xca01,%edi
  40259c:       c1 e8 03                shr    $0x3,%eax
  40259f:       35 a2 08 00 00          xor    $0x8a2,%eax
  4025a4:       01 f8                   add    %edi,%eax
  4025a6:       c3                      ret
  4025a7:       66 0f 1f 84 00 00 00    nopw   0x0(%rax,%rax,1)
  4025ae:       00 00 
```
这是一段简单的计算，并没有复杂的结构。现在已经推导出了3个输出，现在只需要用gdb确认输入格式
```
(gdb) x/s 0x40317c
0x40317c:       "%u %u %u"
```
确定格式后还需要注意，因为字符串最短长度是36，所以需要在长度短于36的输入尾加空格。

## 测试
### 公有测试
![全部通过](./images/2026-10-30%20230034.png)全部通过
### 个人测试
![我使用的种子](./images/2026-10-30%20230500.png)我使用的种子
![phase1](./images/2026-10-30%20230516.png)
![phase2](./images/2026-10-30%20230524.png)
![phase3](./images/2026-10-30%20230534.png)
![phase4](./images/2026-10-30%20230545.png)
![phase5](./images/2026-10-30%20230554.png)
![phase6](./images/2026-10-30%20230602.png)
![secret phase](./images/2026-10-30%20230640.png)
全部通过

## 总结
- bomblab的前6题结构都非常类似，我自己归纳如下：先静态出每个phase的主函数并加以分析。主函数会明确参数个数，调用的函数接口，和一些对参数的简单处理。接下来需要仔细看被调用函数的编译码，这些往往都是具体实现的核心。简单的题基本上就是一些计算，难题会出现循环，switch，判断，递归。对于调用一连串函数的结构，使用gdb动态分析，找出函数调用的顺序，然后再看函数的内容。
- phase1-6的重点分别是：熟悉函数调用和编译语言的各种计算，循环，switch，循环递归判断，顺序链表调用，连续连个函数表的调用。要准确识别出他们，需要对编译码的特定指令非常熟悉。
- 我的报告重点分析了每个phase的算法结构，但是具体计算我并没有写很多，刚开始接触编译语言，需要多读多想才能熟悉利用寄存器和编译语言进行的计算
- secret phase有三个主函数，其实是被很好包装过的，实际重要的只有一个secret phase对他的分析和前面六题就很像了。
- 对编译码，反汇编等有了初步的概念，不仅学到了一门新的语言，更是对各种数据结构和c语言关系语句有了更深刻的认识。