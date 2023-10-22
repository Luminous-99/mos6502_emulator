#pragma once
#define TOGGLE_FLAG(expr,flag) if(expr) cpu->status |= flag; \
                               else  cpu->status &= ~flag;
#define IS_SET(flag) (cpu->status & flag ? 1 : 0)
