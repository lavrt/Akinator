#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "structures.h"
#include "akinator.h"

int main()
{
    tNode* root = createNode();

    databaseFetch(root);

    runAkinator(root);

    dump(root);

    databaseSave(root);

    treeDtor(root);

    return 0;
}
