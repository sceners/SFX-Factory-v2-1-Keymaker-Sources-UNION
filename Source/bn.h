//
// This is only the header of BN functions
// You can use any library, e.g., MIRACL or SSLeay, to replace them.
//

typedef unsigned long BN;

static void BN_Mov( BN *A, BN *B );
static void BN_sMov( BN *A, BN b );
static int BN_Cmp( BN *A, BN *B );
static int BN_Add( BN *A, BN *B );
static int BN_Sub( BN *A, BN *B );
static int BN_Shl( BN *A, BN Sign );
static void BN_MulMod( BN *A, BN *B, BN *C, BN *D );
static void BN_ExpMod( BN *A, BN *B, BN *C, BN *D );
static BN BN_sDiv( BN *A, BN b );
