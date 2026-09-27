extern void InitTLB(void);
extern void RfuDispatchEntry(int status);
extern void JumpToRfuDispatch(void);
extern void _Exit(int status);

void JumpToRfuStatus(void)
{
    RfuDispatchEntry(0);
}
