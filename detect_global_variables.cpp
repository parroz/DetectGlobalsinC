#include <clang-c/Index.h>
#include <stdio.h>
#include <string.h>

static const char* SYSTEM_INCLUDE_PATH = "/usr/include";
static const char* SDK_INCLUDE_PATH = "/Library/Developer/CommandLineTools/SDKs";

static int globalVariableDetected = 0;

static CXChildVisitResult findGlobalVariables(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(cursor) == CXCursor_VarDecl) {
        // Check if the variable is a function parameter
        CXCursor parentCursor = clang_getCursorSemanticParent(cursor);
        if (clang_getCursorKind(parentCursor) == CXCursor_FunctionDecl) {
            return CXChildVisit_Continue;
        }

        CXSourceLocation location = clang_getCursorLocation(cursor);
        CXFile file;
        unsigned int line, column, offset;
        clang_getFileLocation(location, &file, &line, &column, &offset);
        const char* filename = clang_getCString(clang_getFileName(file));

        // Ignore variables defined in system libraries
        if (strstr(filename, SYSTEM_INCLUDE_PATH) != NULL || strstr(filename, SDK_INCLUDE_PATH) != NULL) {
            return CXChildVisit_Continue;
        }

        printf("Variable: %s\n", clang_getCString(clang_getCursorSpelling(cursor)));
        printf("  Location: %s:%u:%u\n", filename, line, column);

        globalVariableDetected = 1;
    }

    return CXChildVisit_Recurse;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please provide one or more file names as command-line arguments.\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        const char* filename = argv[i];
        printf("Checking file: %s\n", filename);

        CXIndex index = clang_createIndex(0, 0);
        CXTranslationUnit translationUnit = clang_parseTranslationUnit(
            index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);

        if (translationUnit == NULL) {
            printf("Error parsing translation unit.\n");
            return 1;
        }

        CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
        clang_visitChildren(cursor, findGlobalVariables, NULL);

        clang_disposeTranslationUnit(translationUnit);
        clang_disposeIndex(index);
    }

    if (globalVariableDetected) {
        return 1;
    }

    return 0;
}
