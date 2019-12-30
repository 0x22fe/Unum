#include "Unum.h"

// C++ handler
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
 * Reporting functions
 ****************************************************************/

// Returns time in microseconds
// https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/
UNUM_DEF size Unum_Internal_Time(void)
{
    return (size)(((f64)(clock()) / (f64)(CLOCKS_PER_SEC)) * 1e6);
}

UNUM_DEF void Unum_Internal_Exception(UnumInstance* c, str msg, UnumCode code, size tok)
{
    c->result.code = code;
    c->result.status = UNUM_CODES[code].level;
    c->result.message = msg;
    c->result.location = tok;
    c->result.result = UNUM_CODES[code].result;

    unum_log("Result: %s (%s), token %u - \"%s\"",
             UNUM_STATUSES[c->result.status], UNUM_CODES[c->result.code].message,
            c->result.location, c->result.message);
}

UNUM_DEF bool Unum_Internal_Valid(any p)
{
    if(p == NULL || p == SAFE)
        return UNUM_FALSE;
    else
        return UNUM_TRUE;
}

/****************************************************************
 * Utility and Parsing
 ****************************************************************/

UNUM_DEF bool Unum_Internal_Utility_Txtvalid(txt s)
{
    if(s == '\0' || (iscntrl(s) && s != '\n' && s != '\t')) return UNUM_FALSE;
    else return UNUM_TRUE;
}

UNUM_DEF bool Unum_Internal_Utility_Strstart(str s, str starts)
{
    if(strncmp(s, starts, strlen(starts)) == 0) return UNUM_TRUE;
    else return UNUM_FALSE;
}

UNUM_DEF str Unum_Internal_Utility_Strdup(str s)
{
    size len = ((size)strlen(s)) + 1;
    str res = UNUM_MALLOC(sizeof(txt) * len);
    memcpy(res, s, len);
    res[len - 1] = '\0';
    return res;
}

UNUM_DEF str Unum_Internal_Utility_Stradd(str a, str b)
{
    size lena = (size)strlen(a);
    size lenb = (size)strlen(b);
    str res = UNUM_MALLOC(sizeof(txt) * (lena + lenb));
    memcpy(res, a, lena);
    memcpy(res + (lena * sizeof(txt)), b, lenb);
    return res;
}

UNUM_DEF str Unum_Internal_Utility_Strsub(str s, size a, size b)
{
    size len = b - a;
    str res = UNUM_MALLOC(sizeof(txt) * len);
    memcpy(res, s + (a * sizeof(txt)), len);
    return res;
}

UNUM_DEF str Unum_Internal_Utility_Strins(str s, str location, str a)
{
    size lens = (size)strlen(s);
    size lena = (size)strlen(a);

    if(s > location || s + (lens * sizeof(txt)) < location)
        return NULL;

    size delta = (size)(location - s);

    str res = UNUM_MALLOC(lens + lena);
    memset(res, 0, lens + lena);
    memcpy(res, s, delta);
    memcpy(res + delta, a, lena);
    memcpy(res + delta + lena, s + delta, lens - delta);

    return res;
}

UNUM_DEF UnumInternalTokens Unum_Internal_Utility_Tokensub(UnumInternalTokens s, UnumInternalPair range)
{
    UnumInternalTokens res;
    res.code = NULL;
    res.statement = NULL;
    res.count = (range.b - range.a) + 1;
    res.tokens = UNUM_MALLOC(sizeof(UnumInternalToken) * res.count);

    for(size i = 0; i < res.count; i++)
    {
        res.tokens[i] = s.tokens[range.a + i];
    }

    return res;
}

UNUM_DEF str Unum_Internal_Parse_Char(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(start[i] == '\'' && start[i - 1] != '\\') break;
        i++;
    }

    i++;

    return start + (i * sizeof(txt));
}

UNUM_DEF str Unum_Internal_Parse_String(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(start[i] == '"' && start[i - 1] != '\\') break;
        i++;
    }

    i++;

    return start + (i * sizeof(txt));
}

UNUM_DEF str Unum_Internal_Parse_Comment(str start)
{
    size i = 1;

    // Multi-line comment
    if(start[i] == '#') {
        i++;
        while(Unum_Internal_Utility_Txtvalid(start[i]))
        {
            if(start[i - 1] == '#' && start[i] == '#') break;
            i++;
        }
        i++;
    }
    // Single-line comment
    else {
        while(Unum_Internal_Utility_Txtvalid(start[i]))
        {
            if(start[i] == '\n') break;
            i++;
        }
    }

    return start + (i * sizeof(txt));
}

UNUM_DEF str Unum_Internal_Parse_Symbol(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(!isalnum(start[i]) && start[i] != '_') break;
        i++;
    }

    return start + (i * sizeof(char));
}

UNUM_DEF str Unum_Internal_Parse_Word(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(!isalnum(start[i]) && start[i] != '_') break;
        i++;
    }

    return start + (i * sizeof(char));
}

UNUM_DEF str Unum_Internal_Parse_Number(str start)
{
    size i = 1;

    while(Unum_Internal_Utility_Txtvalid(start[i]))
    {
        if(!isdigit(start[i]))
        {
            if(start[i] == '.')
            {
                if(!isdigit(start[i + 1])) break;
            } else {
                break;
            }
        }

        i++;
    }

    return start + (i * sizeof(char));
}

UNUM_DEF str Unum_Internal_Parse_Operator(str start)
{
    str begin = start;

    // Todo - Optimize
    if(Unum_Internal_Utility_Strstart(begin, "{") ||
            Unum_Internal_Utility_Strstart(begin, "}") ||
            Unum_Internal_Utility_Strstart(begin, "(") ||
            Unum_Internal_Utility_Strstart(begin, ")") ||
            Unum_Internal_Utility_Strstart(begin, "[") ||
            Unum_Internal_Utility_Strstart(begin, "]") ||
            Unum_Internal_Utility_Strstart(begin, ",") ||
            Unum_Internal_Utility_Strstart(begin, ".") ||
            Unum_Internal_Utility_Strstart(begin, ":") ||
            Unum_Internal_Utility_Strstart(begin, ";")) {
        return begin + (1 * sizeof(txt));
    } else if (Unum_Internal_Utility_Strstart(begin, "->")) {
        return begin + (2 * sizeof(txt));
    } else {
        return NULL;
    }
}

UNUM_DEF UnumInternalTokens Unum_Internal_Tokenize(str code)
{
    size i = 0;
    size u = 0;
    size lines = 0;
    size length = (size)strlen(code);

    UnumInternalTokens result;

    result.count = 0;
    result.code = UNUM_MALLOC(length * sizeof(txt));
    result.statement = UNUM_MALLOC(2 * length * sizeof(txt)); // Arbitrary size
    result.tokens = UNUM_MALLOC(sizeof(UnumInternalToken) * length); // Likely exceeds actual count

    memcpy(result.code, code, length * sizeof(txt));
    memset(result.statement, ' ', 2 * length * sizeof(txt));
    memset(result.tokens, 0, sizeof(UnumInternalToken) * length);

    // Parse loop
    while(i < length)
    {
        str start = NULL;
        str end = NULL;

        UnumInternalLang val = UNUM_LANG_BLANK;
        bool skip = UNUM_FALSE;

        if(result.code[i] == '\'')
        {
            // Char
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Char(start);
            val = UNUM_LANG_CHAR;
        } else if(result.code[i] == '"')
        {
            // String
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_String(start);
            val = UNUM_LANG_STRING;
        } else if(result.code[i] == '#') {
            // Comment
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Comment(start);
            skip = UNUM_TRUE;
        } else if(result.code[i] == '@') {
            // Keyword
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Symbol(start);
            val = UNUM_LANG_KEYWORD;
        } else if(result.code[i] == '$') {
            // Word (symbol value)
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Word(start);
            val = UNUM_LANG_WORD;
        } else if(isalpha(result.code[i]) || result.code[i] == '_') {
            // Symbol
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Symbol(start);
            val = UNUM_LANG_SYMBOL;
        } else if(isdigit(result.code[i]) || (result.code[i] == '.' && isdigit(result.code[i + 1]))
                  || (result.code[i] == '-' && ((result.code[i + 1] == '.' && isdigit(result.code[i + 2]))
                                                || isdigit(result.code[i + 1])))) {
            // Number
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Number(start);
            val = UNUM_LANG_NUMBER;
        } else if(!isspace(result.code[i])) {
            // Operator
            start = result.code + (i * sizeof(txt));
            end = Unum_Internal_Parse_Operator(start);
            val = UNUM_LANG_OPERATOR;
        } else {
            // Whitespace
            skip = UNUM_TRUE;
            while(isspace(result.code[i]) && Unum_Internal_Utility_Txtvalid(result.code[i]))
            {
                if(result.code[i] == '\n') lines++;
                i++;
            }
        }

        // Add to tokens
        if(start != NULL)
        {
            if(end == NULL) /* Error */ return result;

            if(!skip)
            {
                str s = result.statement + (u * sizeof(txt));

                memcpy(s, start, (size_t)(end - start));

                result.tokens[result.count].type = val;
                result.tokens[result.count].value = s;

                u += (size_t)(end - start) / sizeof(txt);
                result.statement[u] = '\0';
                u++;

                result.count++;
            } else {
                lines++;
            }

            i += (size_t)(end - start) / sizeof(txt);
        }
    }

    return result;
}

UNUM_DEF UnumInternalPair Unum_Internal_Parse_Match(UnumInternalTokens t, size start)
{
    const UnumInternalPair def = (UnumInternalPair){.a = 0, .b = 0};

    str begin = t.tokens[start].value;
    str match;

    if(strcmp(begin, "{") == 0) match = "}"; // Set operator
    else if(strcmp(begin, "(") == 0) match = ")"; // Expression operator
    else if(strcmp(begin, "[") == 0) match = "]"; // Array operator
    else return def;

    size open = 1;
    for(size i = start + 1; i < t.count; i++)
    {
        if(strcmp(t.tokens[i].value, match) == 0) open--;
        else if (strcmp(t.tokens[i].value, begin) == 0) open++;
        if(open == 0) return (UnumInternalPair){.a = start, .b = i};
    }

    return def;
}

// If range.a is not equal to range.b, the range values are used
// If range.a is equal to range.b, it is ignored
UNUM_DEF UnumInternalPairList Unum_Internal_Parse_Separate(UnumInternalTokens t, UnumInternalPair range, str sep, bool allow_skip)
{
    const UnumInternalPairList def = (UnumInternalPairList){.num = 0, .pairs = NULL};

    UnumInternalPair p;
    UnumInternalPairList res;
    res.num = 0;
    res.pairs = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);

    if(range.a > range.b)
    {
        UNUM_FREE(res.pairs);
        res.pairs = SAFE;
        return res;
    }

    Unum_Debug_Sequence(t, range);

    if(range.a != range.b)
    {
        // Parse in range
        p = range;
    } else {
        res.num = 1;
        res.pairs[0] = range;
        return res;
    }

    // Determines if token pair needs to be skipped
    bool skip = UNUM_FALSE;
    if(allow_skip && (
                strcmp(t.tokens[p.a].value, "{") == 0 ||
                strcmp(t.tokens[p.a].value, "(") == 0 ||
                strcmp(t.tokens[p.a].value, "[") == 0
                ))
    {
        UnumInternalPair match = Unum_Internal_Parse_Match(t, p.a);
        if(match.b == p.b)
            skip = UNUM_TRUE;
    }

    // Skips first token (usually a bracket/brace/parenthesis)
    size i = skip ? p.a + 1 : p.a;
    size last = i; //skip ? p.a : p.a;

    while(i <= p.b)
    {
        if(strcmp(t.tokens[i].value, "{") == 0 || strcmp(t.tokens[i].value, "(") == 0 || strcmp(t.tokens[i].value, "[") == 0)
        {
            UnumInternalPair tmp = Unum_Internal_Parse_Match(t, i);
            if(tmp.a == tmp.b || tmp.b > p.b)
            {
                // Error - Incorrect start/end operators (syntax)
                UNUM_FREE(res.pairs);
                return def;
            }
            i = tmp.b;
        }

        if(strcmp(t.tokens[i].value, sep) == 0)
        {
            /*
            if(last >= i - 1) {
                res.num++;
                res.pairs = UNUM_REALLOC(res.pairs, sizeof(UnumInternalPair) * res.num);
                res.pairs[res.num - 1] = (UnumInternalPair) {.a = i - 1, .b = i - 1};
            }
            */
            res.num++;
            res.pairs = UNUM_REALLOC(res.pairs, sizeof(UnumInternalPair) * res.num);
            res.pairs[res.num - 1] = (UnumInternalPair) {.a = last, .b = i - 1};
            last = i + 1;
        }

        i++;
    }

    if(last == i + 1) {
        // Warning - Stray separator at end of array
        // Ignore, as this is helpful in cases where the array/set/expression is generated by an external program or script
    } else {
        // Add last element
        res.num++;
        res.pairs = UNUM_REALLOC(res.pairs, sizeof(UnumInternalPair) * res.num);
        // Determine end index
        if(skip) // strcmp(sep, ";") == 0)
        {
            res.pairs[res.num - 1] = (UnumInternalPair) {.a = last, .b = p.b - 1 };
        } else {
            res.pairs[res.num - 1] = (UnumInternalPair) {.a = last, .b = p.b };
        }

        // Handle special cases where function is called at end of program
        if(res.pairs[res.num - 1].a > res.pairs[res.num - 1].b)
        {
            res.num = 0;
            UNUM_FREE(res.pairs);
            res.pairs = SAFE;
        }
    }

    return res;
}

UNUM_DEF void Unum_Debug_Pair(UnumInternalTokens s, UnumInternalPairList p)
{
    unum_log("Pair (%u element(s)):", p.num);
#ifdef UNUM_DEBUG
    for(size t = 0; t < p.num; t++)
    {
        printf("\t\t");
        if(p.pairs[t].a == p.pairs[t].b)
        {
            printf("%s", s.tokens[p.pairs[t].a].value);
        } else {
            for(size c = p.pairs[t].a; c <= p.pairs[t].b; c++)
                printf("%s", s.tokens[c].value);
        }
        printf("\n");
    }
#endif
    unum_log("End Pair");
}

UNUM_DEF void Unum_Debug_Sequence(UnumInternalTokens s, UnumInternalPair p)
{
    unum_log("Sequence (%u token(s)):", p.b - p.a);
#ifdef UNUM_DEBUG
    printf("\t\t");
    for(size t = p.a; t <= p.b; t++)
    {
        printf("%s", s.tokens[t].value);
    }
    printf("\n");
#endif
    unum_log("End Sequence");
}

// Todo - Finish
UNUM_DEF void Unum_Debug_Object(UnumInternalTokens s, UnumInternalObject p, size offset)
{
    for(size o = 0; o < offset; o++) printf("\t");
    printf("\nObject \"%s\", value %p, type %u: ", Unum_Internal_Valid(p.name) ? p.name : "NULL/SAFE", p.data, (size)p.type);
#ifdef UNUM_DEBUG
    if(p.type == UNUM_OBJ_SYMBOL)
    {
        UnumInternalObjSymbol n = *((UnumInternalObjSymbol*)p.data);
        for(size o = 0; o < offset; o++) printf("\t");
        printf("Symbol - %u Name(s)\n", n.count);
        for(size i = 0; i < n.count; i++)
        {
            for(size o = 0; o < offset; o++) printf("\t");
            if(i != n.count - 1) printf("%s.", Unum_Internal_Valid(n.names[i]) ? n.names[i] : "NULL/SAFE");
            else printf("%s\n", n.names[i]);
        }
    } else if(p.type == UNUM_OBJ_STACK) {
        UnumInternalObjStack n = *((UnumInternalObjStack*)p.data);
        for(size o = 0; o < offset; o++) printf("\t");
        printf("Stack - %u Object(s)\n", n.count);
        for(size i = 0; i < n.count; i++)
        {
            for(size o = 0; o < offset; o++) printf("\t");
            printf("Index %u: ", i);
            Unum_Debug_Object(s, n.objects[i], offset + 1);
        }
    } else if(p.type == UNUM_OBJ_BLANK) {
        for(size o = 0; o < offset; o++) printf("\t");
        printf("Blank - Null\n");
    } else {
        for(size o = 0; o < offset; o++) printf("\t");
        printf("Unknown\n");
    }
#endif
    for(size o = 0; o < offset; o++) printf("\t");
    printf("End Object\n");
}

/****************************************************************
 * Language core functions
 ****************************************************************/

UNUM_DEF bool Unum_Internal_Object_Safe(UnumInternalObject o)
{
    if(o.name == SAFE && o.data == SAFE && o.type == UNUM_OBJ_BLANK)
        return UNUM_TRUE;
    else
        return UNUM_FALSE;
}

UNUM_DEF bool Unum_Internal_Object_Null(UnumInternalObject o)
{
    if(o.name == NULL && o.data == NULL && o.type == UNUM_OBJ_BLANK)
        return UNUM_TRUE;
    else
        return UNUM_FALSE;
}

// Todo - Finish (not completed)
// Recursively clears an object (frees memory)
UNUM_DEF void Unum_Internal_Object_Destroy(UnumInternalObject o)
{
    if(Unum_Internal_Valid(o.name))
        UNUM_FREE(o.name);
    if(Unum_Internal_Valid(o.data))
        UNUM_FREE(o.data);
}

// Todo - Finish (not usable)
// Compares the types of two objects
UNUM_DEF bool Unum_Internal_Object_Type(UnumInternalObject a, UnumInternalObject b)
{
    if(a.type == b.type)
    {
        switch (a.type)
        {
        case UNUM_OBJ_BLANK: {
            // Blank type comparison is always false
            return UNUM_FALSE;
        }
        case UNUM_OBJ_TYPE: {
            UnumInternalObjType* at = (UnumInternalObjType*)a.data;
            UnumInternalObjType* bt = (UnumInternalObjType*)b.data;

            if(strcmp(at->type, bt->type) != 0)
            {
                return UNUM_FALSE;
            }
            else {
                if(at->count != bt->count)
                {
                    return UNUM_FALSE;
                } else {
                    for(size i = 0; i < at->count; i++)
                    {
                        // ...
                    }
                }
            }
        }
        case UNUM_OBJ_STACK: {
            return UNUM_FALSE;
        }
        case UNUM_OBJ_SYMBOL: {
            UnumInternalObjSymbol* at = (UnumInternalObjSymbol*)a.data;
            UnumInternalObjSymbol* bt = (UnumInternalObjSymbol*)b.data;

            // ...

            return UNUM_FALSE;
        }
        case UNUM_OBJ_SINGLE: {
            return UNUM_FALSE;
        }
        case UNUM_OBJ_ARRAY: {
            return UNUM_FALSE;
        }
        case UNUM_OBJ_SET: {
            return UNUM_FALSE;
        }
        case UNUM_OBJ_EXPRESSION: {
            return UNUM_FALSE;
        }
        case UNUM_OBJ_FUNCTION: {
            return UNUM_FALSE;
        }
        case UNUM_OBJ_STRUCTURE: {
            return UNUM_FALSE;
        }
        case UNUM_OBJ_NAMESPACE: {
            return UNUM_FALSE;
        }
        case UNUM_OBJ_ALIAS: {
            return UNUM_FALSE;
        }
        default: {
            // Non-covered enum type
            return UNUM_FALSE;
        }
        }
    } else {
        return UNUM_FALSE;
    }
}

// Returns an object based on the given object
UNUM_DEF UnumInternalObject Unum_Internal_Object_Bool(UnumInternalObject o)
{
    if(o.type == UNUM_OBJ_SINGLE)
    {
        bool val = UNUM_FALSE;
        UnumInternalObjSingle* s = (UnumInternalObjSingle*)o.data;

        // Boolean type
        if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_BOOL].type) == 0)
            return o;
        // Unsigned integer types
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_U8].type) == 0)
            val = *((u8*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_U16].type) == 0)
            val = *((u16*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_U32].type) == 0)
            val = *((u32*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_U64].type) == 0)
            val = *((u64*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        // Signed integer types
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_I8].type) == 0)
            val = *((i8*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_I16].type) == 0)
            val = *((i16*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_I32].type) == 0)
            val = *((i32*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_I64].type) == 0)
            val = *((i64*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        // Floating point types
        // Todo - Check for NaN and Inf as well
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_F32].type) == 0)
            val = *((f32*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        else if(strcmp(s->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_F64].type) == 0)
            val = *((f64*)s->data) > 0 ? UNUM_TRUE : UNUM_FALSE;
        else
            return UNUM_OBJ_DEF;

        // Create boolean object
        UnumInternalObjSingle* os = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);
        os->data = UNUM_MALLOC(sizeof(bool) * 1);
        os->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_BOOL];
        *((bool*)os->data) = val;

        UnumInternalObject o;
        o.name = SAFE;
        o.data = os;
        o.type = UNUM_OBJ_SINGLE;

        return o;
    } else {
        return UNUM_OBJ_DEF;
    }
}

// Resolves object name in stack
UNUM_DEF size Unum_Internal_Stack_Id(UnumInternalObjStack* c, str name)
{
    if(!Unum_Internal_Valid(name))
        return 0;

    // Todo - Optimize by using a hash table
    for(size i = 1; i < c->count; i++)
    {
        if(Unum_Internal_Valid(c->objects[i].name))
        {
            if(strcmp(name, c->objects[i].name) == 0)
            {
                return i;
            }
        }
    }

    return 0;
}

// Resolves object name to stack level and ID
UNUM_DEF UnumInternalPair Unum_Internal_Execute_Id(UnumInstance* c, str name)
{
    // Lookup object name in all stack levels

    UnumInternalPair res;
    res.a = 0; // Stack level
    res.b = 0; // Object ID

    if(!Unum_Internal_Valid(name))
        return res;

    // Start from highest level of stack
    for(isize i = Unum_Internal_Execute_Stack_Level(c); i >= 0; i--)
    {
        // Search for match in current stack
        res.b = Unum_Internal_Stack_Id(Unum_Internal_Execute_Level(c, i), name);

        // Set stack level
        res.a = i;

        // Handle match
        if(res.b != 0)
            break;
    }

    /*
    UnumInternalObjStack* last = Unum_Internal_Execute_Stack_Level(c);
    while(Unum_Internal_Valid(last))
    {
        // Search for match in current stack
        res.b = Unum_Internal_Stack_Id(last, name);

        // Handle match
        if(res.b != 0)
            break;

        // Increment stack level
        res.a++;

        // Element at location 0 should be a stack object
        if(last->objects[0].type == UNUM_OBJ_STACK)
            last = (UnumInternalObjStack*)(last->objects[0].data);
        else
            break;
        // Next stack
        last = (UnumInternalObjStack*)(&last->objects[0]);
    }
        */

    return res;
}

// Returns highest stack level
UNUM_DEF size Unum_Internal_Execute_Stack_Level(UnumInstance* c)
{
    size level = 0;
    UnumInternalObjStack* last = &c->stack;
    while(last->objects != NULL)
    {
        last = (UnumInternalObjStack*)(&last->objects[0]);
        level++;
    }
    return level;
}

// Returns pointer to highest level stack
UNUM_DEF UnumInternalObjStack* Unum_Internal_Execute_Stack(UnumInstance* c)
{
    UnumInternalObjStack* last = &c->stack;
    while(last->objects != NULL && !Unum_Internal_Object_Null(last->objects[0]))
        last = (UnumInternalObjStack*)(&last->objects[0]);
    return last;
}

// Returns pointer to given level stack
UNUM_DEF UnumInternalObjStack* Unum_Internal_Execute_Level(UnumInstance* c, size lvl)
{
    size i = 0;
    UnumInternalObjStack* last = &c->stack;
    while(last->objects != NULL && i < lvl)
    {
        i++;
        last = (UnumInternalObjStack*)(&last->objects[0]);
    }
    return last;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Function(UnumInstance* c, UnumInternalObjStack* params)
{
    if(params->count == 0)
    {
        Unum_Internal_Exception(c, "Not enough arguments (need function name)", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->count > 2)
    {
        Unum_Internal_Exception(c, "Too many arguments (only need function name)", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[1].type != UNUM_OBJ_SYMBOL)
    {
        Unum_Internal_Exception(c, "Argument is not a symbol", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Verify symbol name doesn't already exist
    // Idea - Allow for inline namespaces here, for now just use the first name
    str name = ((UnumInternalObjSymbol*)params->objects[1].data)->names[0];
    UnumInternalPair id_res = Unum_Internal_Execute_Id(c, name);

    // Check if there is a match and if it is on another stack level
    if(id_res.b != 0 && Unum_Internal_Execute_Level(c, id_res.a) != params)
    {
        Unum_Internal_Exception(c, "Duplicate symbol name", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Create function object

    UnumInternalObjFunction* f = UNUM_MALLOC(sizeof(UnumInternalObjFunction) * 1);
    f->body = (UnumInternalObjExpression) { .pair = (UnumInternalPair) {.a = 0, .b = 0} };
    f->result = (UnumInternalObjType) { .type = NULL, .count = 0, .parts = NULL};
    f->parameters = (UnumInternalObjSet) { .count = 0, .keys = NULL, .values = NULL};

    UnumInternalObject o;
    o.data = f;
    o.name = Unum_Internal_Utility_Strdup(name);
    o.type = UNUM_OBJ_FUNCTION;

    unum_log("New Function \"%s\"", o.name);
    return o;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Parameters(UnumInstance* c, UnumInternalObjStack* params)
{
    // Remember, 0 is reserved in the stack
    if(params->count < 2 || params->count > 3)
    {
        Unum_Internal_Exception(c, "@parameters needs to be called after a @function pass and an optional set", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[1].type != UNUM_OBJ_FUNCTION)
    {
        Unum_Internal_Exception(c, "@parameters called with pass not from @function", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // No argument function
    if(params->count == 2)
    {
        // Maybe Todo - Return empty parameter object
        ((UnumInternalObjFunction*)params->objects[1].data)->parameters.count = 0;
        ((UnumInternalObjFunction*)params->objects[1].data)->parameters.keys = SAFE;
        ((UnumInternalObjFunction*)params->objects[1].data)->parameters.values = SAFE;
        unum_log("No Parameters");
        return params->objects[1];
    }

    // Function with argument(s)
    if(params->objects[2].type != UNUM_OBJ_SET)
    {
        Unum_Debug_Object(c->program, params->objects[2], 0);
        Unum_Internal_Exception(c, "@parameters expected a set as a second argument", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Add parameters to function object
    UnumInternalObjSet* args = (UnumInternalObjSet*)params->objects[2].data;
    ((UnumInternalObjFunction*)params->objects[1].data)->parameters = *args;

    // Verify set is in correct format
    for(size u = 0; u < args->count; u++)
    {
        // Get set properties
        str current_name = args->keys[u];
        UnumInternalObject current_obj = args->values[u];

        // Verify object is of "type" type
        if(current_obj.type != UNUM_OBJ_TYPE)
        {
            Unum_Internal_Exception(c, "@parameters argument set expected type name", UNUM_CODE_RUNTIME_ERROR, 0);
            return UNUM_OBJ_DEF;
        }

        // Lookup typename
        // Todo - Support more than the "Single" type
        UnumInternalPair param_type_id = Unum_Internal_Execute_Id(c, ((UnumInternalObjType*)current_obj.data)->type);

        // Verify type exists
        if(param_type_id.b == 0)
        {
            Unum_Internal_Exception(c, "@parameters argument set has unknown type", UNUM_CODE_RUNTIME_ERROR, 0);
            return UNUM_OBJ_DEF;
        }

        // Compare with other parameter symbol names to ensure there are no duplicates
        for(size v = 0; v < u; v++)
        {
            if(strcmp(current_name, args->keys[v]) == 0)
            {
                return UNUM_OBJ_DEF;
            }
        }
    }

    unum_log("New Parameters (%u)", args->count);

    /*
        for(size v = 0; v < p.count; v++)
        {
            unum_log("\"%s\" of type ID #%u (\"%s\")", p.parts[v].name, p.parts[v].type_id, c->stack.types[p.parts[v].type_id].name);
        }
    */

    // Return function object
    return params->objects[1];
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Result(UnumInstance* c, UnumInternalObjStack* params)
{
    // Remember, 0 is reserved in the stack
    if(params->objects[1].type != UNUM_OBJ_FUNCTION)
    {
        Unum_Internal_Exception(c, "@result called with pass not from @function", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // No return value
    if(params->count == 2)
    {
        // Maybe Todo - Return empty result object
        ((UnumInternalObjFunction*)params->objects[1].data)->result.count = 0;
        ((UnumInternalObjFunction*)params->objects[1].data)->result.type = SAFE;
        ((UnumInternalObjFunction*)params->objects[1].data)->result.parts = SAFE;
        unum_log("No result");
        return params->objects[1];
        // Has a return value
    } else if (params->count == 3) {
        // Todo - Allow returning custom set instead of a single type
        if(params->objects[2].type != UNUM_OBJ_TYPE)
        {
            Unum_Internal_Exception(c, "@result expected a symbol, got an other type", UNUM_CODE_SYNTAX_ERROR, 0);
            return UNUM_OBJ_DEF;
        }

        // Add result type to function object
        ((UnumInternalObjFunction*)params->objects[1].data)->result = *((UnumInternalObjType*)params->objects[2].data);

        unum_log("New result with type \"%s\"", ((UnumInternalObjType*)params->objects[2].data)->type);

        // Return function object
        return params->objects[1];
    } else {
        // Too many return values, error
        Unum_Internal_Exception(c, "@result has too many return values (only 1 is allowed)", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Namespace(UnumInstance* c, UnumInternalObjStack* params)
{
    // Namespace is a special keyword
    // It expects a name and expression range, not the result of the expression

    // Remember, 0 is reserved in the stack
    if(params->count != 3)
    {
        Unum_Internal_Exception(c, "@namespace needs to be called with a name and expression", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[1].type != UNUM_OBJ_SYMBOL)
    {
        Unum_Internal_Exception(c, "@namespace expected symbol", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Get range of expression using internal data type
    if(params->objects[2].type != UNUM_OBJ_EXPRESSION)
    {
        Unum_Internal_Exception(c, "@namespace expected expression", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Create namespace
    // Idea - Allow for inline namespaces, for now just use first symbol
    UnumInternalObjNamespace* ns = UNUM_MALLOC(sizeof(UnumInternalObjNamespace) * 1);
    ns->name = Unum_Internal_Utility_Strdup(((UnumInternalObjSymbol*)params->objects[1].data)->names[0]);
    ns->stack.count = 1;
    ns->stack.objects = UNUM_MALLOC(sizeof(UnumInternalObject) * ns->stack.count);

    // Execute expression
    // This will fill the stack and return the final object
    UnumInternalObject o = Unum_Internal_Execute_Expressions(c, &ns->stack, ((UnumInternalObjExpression*)params->objects[2].data)->pair);

    // Verify object
    if(Unum_Internal_Object_Null(o))
    {
        UNUM_FREE(ns->stack.objects);
        UNUM_FREE(ns);
        // Error occurred
        return UNUM_OBJ_DEF;
    }

    // Create object
    UnumInternalObject obj;
    obj.name = Unum_Internal_Utility_Strdup(ns->name);
    obj.type = UNUM_OBJ_NAMESPACE;
    obj.data = ns;

    unum_log("Added object \"%s\"", obj.name);

    // Return object
    return obj;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Alias(UnumInstance* c, UnumInternalObjStack* params)
{
    return UNUM_OBJ_DEF;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Structure(UnumInstance* c, UnumInternalObjStack* params)
{
    return UNUM_OBJ_DEF;
}

// Todo - Finish
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Variable(UnumInstance* c, UnumInternalObjStack* params)
{
    // Remember, 0 is reserved in the stack
    if(params->count < 3)
    {
        Unum_Internal_Exception(c, "@variable needs to be called with at least a variable name and type (value is optional)", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[1].type != UNUM_OBJ_SYMBOL)
    {
        Unum_Internal_Exception(c, "@variable expected symbol (variable name)", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[2].type != UNUM_OBJ_TYPE)
    {
        Unum_Internal_Exception(c, "@variable expected type", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Variable declaration
    if(params->count == 3)
    {
        // Maybe Todo - Allow for nested variable declarations
        UnumInternalObject o;
        o.name = Unum_Internal_Utility_Strdup(((UnumInternalObjSymbol*)params->objects[1].data)->names[0]);

        // Todo - Get type from objects[2]
        // For now, only support single variable types
        o.type = UNUM_OBJ_SINGLE;

        // Set object data
        o.data = NULL;

        // Return object
        return o;
        // Variable declaration and definition
    } else if(params->count == 4) {
        // Todo - Verify object matches type given at objects[2]

        // Maybe Todo - Allow for nested variable declarations
        UnumInternalObject o;
        o.name = Unum_Internal_Utility_Strdup(((UnumInternalObjSymbol*)params->objects[1].data)->names[0]);

        // Todo - Get type from objects[2]
        o.type = UNUM_OBJ_SINGLE;

        // For now, only support single variable types
        // Copy object to variable
        UnumInternalObjSingle* os = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);
        *os = *((UnumInternalObjSingle*)params->objects[3].data);

        // Set object data
        o.data = os;

        // Return object
        return o;
    } else {
        // Too many input values, error
        Unum_Internal_Exception(c, "@variable has too many values (only 1 is allowed)", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
}

// Todo - Finish
UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Constant(UnumInstance* c, UnumInternalObjStack* params)
{
    // Remember, 0 is reserved in the stack
    if(params->count != 4)
    {
        Unum_Internal_Exception(c, "@constant needs to be called with a variable name, type, and value", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[1].type != UNUM_OBJ_SYMBOL)
    {
        Unum_Internal_Exception(c, "@constant expected symbol (variable name)", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[2].type != UNUM_OBJ_TYPE)
    {
        Unum_Internal_Exception(c, "@constant expected type", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Constant declaration and definition
    // Todo - Verify object matches type given at objects[2]

    // Maybe Todo - Allow for nested variable declarations
    UnumInternalObject o;
    o.name = Unum_Internal_Utility_Strdup(((UnumInternalObjSymbol*)params->objects[1].data)->names[0]);

    // Todo - Get type from objects[2]
    o.type = UNUM_OBJ_SINGLE;

    // For now, only support single variable types

    // Copy object to variable
    UnumInternalObjSingle* os = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);
    *os = *((UnumInternalObjSingle*)params->objects[3].data);

    // Modify type to be constant
    UnumInternalObjType ot;
    ot.type = "constant";
    ot.count = 1;
    ot.parts = UNUM_MALLOC(sizeof(UnumInternalObjType) * 1);
    ot.parts[0] = os->type;

    // Update object type
    os->type = ot;

    // Set object data
    o.data = os;

    // Return object
    return o;
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Body(UnumInstance* c, UnumInternalObjStack* params)
{
    // Body is a special keyword
    // It expects an expression

    // Remember, 0 is reserved in the stack
    if(params->count != 3)
    {
        Unum_Internal_Exception(c, "@body needs to be called after function pass and with an expression", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[1].type != UNUM_OBJ_FUNCTION)
    {
        Unum_Internal_Exception(c, "@body expected function", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[2].type != UNUM_OBJ_EXPRESSION)
    {
        Unum_Internal_Exception(c, "@body expected expression", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Create expression object
    // Set incoming function object properties
    UnumInternalObjFunction* f = (UnumInternalObjFunction*)params->objects[1].data;
    f->body = *((UnumInternalObjExpression*)params->objects[2].data);

    // Return function object
    return params->objects[1];
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_If(UnumInstance* c, UnumInternalObjStack* params)
{
    UnumInternalObjSingle* ufs = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);
    ufs->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_BOOL];
    ufs->data = UNUM_MALLOC(sizeof(bool) * 1);
    *((bool*)ufs->data) = UNUM_FALSE;

    UnumInternalObjSingle* uts = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);
    uts->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_BOOL];
    uts->data = UNUM_MALLOC(sizeof(bool) * 1);
    *((bool*)uts->data) = UNUM_TRUE;

    UnumInternalObject ufalse;
    ufalse.name = "false";
    ufalse.type = UNUM_OBJ_SINGLE;
    ufalse.data = ufs;

    UnumInternalObject utrue;
    ufalse.name = "true";
    ufalse.type = UNUM_OBJ_SINGLE;
    ufalse.data = uts;

    // Remember, 0 is reserved in the stack

    // No arguments, treat as false
    if(params->count <= 1)
    {
        // Return a false value
        return ufalse;
    }
    // Other combination of arguments
    else
    {
        // Get boolean object
        UnumInternalObject o = Unum_Internal_Object_Bool(params->objects[1]);

        // Verify argument
        if(Unum_Internal_Object_Null(o))
        {
            Unum_Internal_Exception(c, "@if expected single boolean value", UNUM_CODE_SYNTAX_ERROR, 0);
            return UNUM_OBJ_DEF;
        }

        // Boolean object
        bool ov = *((bool*)((UnumInternalObjSingle*)o.data)->data);

        // One argument
        if(params->count == 2)
        {
            // Pass argument
            return params->objects[1];

            // Two or more arguments
        } else {
            // Verify argument is expression
            if(params->objects[2].type != UNUM_OBJ_EXPRESSION)
            {
                Unum_Internal_Exception(c, "@if expected expression", UNUM_CODE_SYNTAX_ERROR, 0);
                return UNUM_OBJ_DEF;
            }

            // Two arguments
            // If true, return the result of the expression
            // If false, return false
            if(params->count == 3)
            {
                // Check if boolean value was true
                if(ov)
                {
                    // Create stack for execution
                    UnumInternalObjStack* s = UNUM_MALLOC(sizeof(UnumInternalObjStack) * 1);
                    s->count = 1;
                    s->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);
                    s->objects[0] = UNUM_OBJ_DEF;

                    // Link stack to parent
                    params->objects[0].name = SAFE;
                    params->objects[0].data = s;
                    params->objects[0].type = UNUM_OBJ_STACK;

                    // Get expression range
                    UnumInternalPair p = *((UnumInternalPair*)params->objects[2].data);

                    // Get result of expression
                    UnumInternalObject res = Unum_Internal_Execute_Data(c, s, p);

                    // Free stack
                    // Todo - Properly clean up stack
                    UNUM_FREE(s);

                    // Return result
                    return res;
                } else {
                    // Return a false value
                    return ufalse;
                }
                // Three arguments
                // If true, return the result of the first expression
                // If false, return the result of the second expression
            } else if(params->count == 4)
            {
                // Create stack for execution
                UnumInternalObjStack* s = UNUM_MALLOC(sizeof(UnumInternalObjStack) * 1);
                s->count = 1;
                s->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);
                s->objects[0] = UNUM_OBJ_DEF;

                // Link stack to parent
                params->objects[0].name = SAFE;
                params->objects[0].data = s;
                params->objects[0].type = UNUM_OBJ_STACK;

                // Check if boolean value was true
                if(ov)
                {
                    // Get first expression range
                    UnumInternalPair p = *((UnumInternalPair*)params->objects[2].data);

                    // Get result of expression
                    UnumInternalObject res = Unum_Internal_Execute_Data(c, s, p);

                    // Free stack
                    // Todo - Properly clean up stack
                    UNUM_FREE(s);

                    // Return result
                    return res;
                } else {
                    // Get second expression range
                    UnumInternalPair p = *((UnumInternalPair*)params->objects[3].data);

                    // Get result of expression
                    UnumInternalObject res = Unum_Internal_Execute_Data(c, s, p);

                    // Free stack
                    // Todo - Properly clean up stack
                    UNUM_FREE(s);

                    // Return result
                    return res;
                }
            } else {
                // Too many arguments
                Unum_Internal_Exception(c, "@if has too many arguments", UNUM_CODE_SYNTAX_ERROR, 0);
                return UNUM_OBJ_DEF;
            }
        }
    }
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Loop(UnumInstance* c, UnumInternalObjStack* params)
{
    // Remember, 0 is reserved in the stack

    // Zero/one arguments
    if(params->count <= 2)
    {
        Unum_Internal_Exception(c, "@loop expected multiple arguments", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    } else {
        // Two arguments
        // First is the number of loops
        // Second is the expression to execute
        if(params->count == 3)
        {
            // First should be unsigned number
            if(params->objects[1].type != UNUM_OBJ_SINGLE)
            {
                Unum_Internal_Exception(c, "@loop expected single value (two arguments)", UNUM_CODE_SYNTAX_ERROR, 0);
                return UNUM_OBJ_DEF;
            }

            u64 count = 0;
            UnumInternalObjSingle* o = (UnumInternalObjSingle*)params->objects[1].data;

            // Todo - Do proper type comparison
            // For now just check first name
            if(strcmp(o->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_U8].type) == 0)
                count = (u64) *((u8*)o->data);
            else if(strcmp(o->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_U16].type) == 0)
                count = (u64) *((u16*)o->data);
            else if(strcmp(o->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_U32].type) == 0)
                count = (u64) *((u32*)o->data);
            else if(strcmp(o->type.type, UNUM_PRIMITIVES[UNUM_PRIMITIVE_U64].type) == 0)
                count = (u64) *((u64*)o->data);
            else {
                Unum_Internal_Exception(c, "@loop expected unsigned number value (two arguments)", UNUM_CODE_SYNTAX_ERROR, 0);
                return UNUM_OBJ_DEF;
            }

            // Second should be expression
            if(params->objects[2].type != UNUM_OBJ_EXPRESSION)
            {
                Unum_Internal_Exception(c, "@loop expected expression (two arguments)", UNUM_CODE_SYNTAX_ERROR, 0);
                return UNUM_OBJ_DEF;
            }

            // Create stack for execution
            UnumInternalObjStack* s = UNUM_MALLOC(sizeof(UnumInternalObjStack) * 1);
            s->count = 1;
            s->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);
            s->objects[0] = UNUM_OBJ_DEF;

            // Link stack to parent
            params->objects[0].name = SAFE;
            params->objects[0].data = s;
            params->objects[0].type = UNUM_OBJ_STACK;

            // Create expression pair and result variables
            UnumInternalPair p = *((UnumInternalPair*)params->objects[2].data);
            UnumInternalObject res = UNUM_OBJ_SAFE;

            // Run loop
            for(u64 i = 0; i < count; i++)
            {
                // Execute expression
                res = Unum_Internal_Execute_Data(c, s, p);

                // Clear stack
                // Todo - Properly clean up stack
                UNUM_FREE(s);

                // Re-create stack
                s = UNUM_MALLOC(sizeof(UnumInternalObjStack) * 1);
                s->count = 1;
                s->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);
                s->objects[0] = UNUM_OBJ_DEF;

                // Re-link stack to parent
                params->objects[0].name = SAFE;
                params->objects[0].data = s;
                params->objects[0].type = UNUM_OBJ_STACK;
            }

            // Clear stack
            // Todo - Properly clean up stack
            UNUM_FREE(s);

            // Return result
            return res;
            // Four arguments
            // First is the initial expression
            // Second is the condition expression
            // Third is the incremental expression
            // Fourth is the main expression
        } else if(params->count == 5) {

            // Verify arguments
            // Todo - Verify each individually to specify problem
            if(params->objects[1].type != UNUM_OBJ_EXPRESSION ||
                    params->objects[2].type != UNUM_OBJ_EXPRESSION ||
                    params->objects[3].type != UNUM_OBJ_EXPRESSION ||
                    params->objects[4].type != UNUM_OBJ_EXPRESSION)
            {
                Unum_Internal_Exception(c, "@loop expected expression(s) (four arguments)", UNUM_CODE_SYNTAX_ERROR, 0);
                return UNUM_OBJ_DEF;
            }

            // Create stack for execution
            UnumInternalObjStack* s = UNUM_MALLOC(sizeof(UnumInternalObjStack) * 1);
            s->count = 1;
            s->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);
            s->objects[0] = UNUM_OBJ_DEF;

            // Link stack to parent
            params->objects[0].name = SAFE;
            params->objects[0].data = s;
            params->objects[0].type = UNUM_OBJ_STACK;

            // Execute initial expression
            UnumInternalPair initialp = *((UnumInternalPair*)params->objects[1].data);
            UnumInternalObject initialr = Unum_Internal_Execute_Data(c, s, initialp);

            // Verify expression
            if(Unum_Internal_Object_Null(initialr))
            {
                Unum_Internal_Exception(c, "@loop had bad initial expression (four arguments)", UNUM_CODE_RUNTIME_ERROR, 0);
                return UNUM_OBJ_DEF;
            }

            // Get condition expression
            bool condition = UNUM_FALSE;
            UnumInternalPair conditionp = *((UnumInternalPair*)params->objects[2].data);
            UnumInternalObject conditionr = Unum_Internal_Execute_Data(c, s, conditionp);
            conditionr = Unum_Internal_Object_Bool(conditionr);

            // Verify result
            if(Unum_Internal_Object_Null(conditionr))
            {
                Unum_Internal_Exception(c, "@loop had bad condition expression, expected single boolean object (four arguments)", UNUM_CODE_RUNTIME_ERROR, 0);
                return UNUM_OBJ_DEF;
            } else {
                condition = *((bool*)((UnumInternalObjSingle*)conditionr.data)->data);
            }

            // Loop counter
            size counter = 0;

            // Get starting stack count
            size start_count = s->count;

            // Start main loop
            while(condition)
            {
                // Execute main expression
                UnumInternalPair mainp = *((UnumInternalPair*)params->objects[4].data);
                UnumInternalObject mainr = Unum_Internal_Execute_Data(c, s, mainp);

                // Verify result
                if(Unum_Internal_Object_Null(mainr))
                {
                    Unum_Internal_Exception(c, "@loop had bad main expression (four arguments)", UNUM_CODE_RUNTIME_ERROR, 0);
                    return UNUM_OBJ_DEF;
                }

                // Clear stack after start point
                // Todo - Properly clean up objects
                for(size c = start_count; c < s->count; c++)
                    s->objects[c] = UNUM_OBJ_DEF;
                s->count = start_count;

                // Execute incremental expression
                UnumInternalPair incrementalp = *((UnumInternalPair*)params->objects[3].data);
                UnumInternalObject incrementalr = Unum_Internal_Execute_Data(c, s, incrementalp);

                // Verify result
                if(Unum_Internal_Object_Null(incrementalr))
                {
                    Unum_Internal_Exception(c, "@loop had bad incremental expression (four arguments)", UNUM_CODE_RUNTIME_ERROR, 0);
                    return UNUM_OBJ_DEF;
                }

                // Execute condition expression
                conditionr = Unum_Internal_Execute_Data(c, s, conditionp);
                conditionr = Unum_Internal_Object_Bool(conditionr);

                // Verify result
                if(Unum_Internal_Object_Null(conditionr))
                {
                    Unum_Internal_Exception(c, "@loop had bad condition expression, expected single boolean object (four arguments)", UNUM_CODE_RUNTIME_ERROR, 0);
                    return UNUM_OBJ_DEF;
                } else {
                    condition = *((bool*)((UnumInternalObjSingle*)conditionr.data)->data);
                }

                // Increment loop counter
                counter++;
            }

            // Create number object
            UnumInternalObjSingle* so = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);
            so->data = UNUM_MALLOC(sizeof(u64) * 1);
            so->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_U64];
            *((u64*)so->data) = counter;

            UnumInternalObject ro;
            ro.name = SAFE;
            ro.type = UNUM_OBJ_SINGLE;
            ro.data = so;

            // Return number of successful iterations
            return ro;

            // Bad number of arguments
        } else {
            Unum_Internal_Exception(c, "@loop expected either 2 or 4 arguments", UNUM_CODE_SYNTAX_ERROR, 0);
            return UNUM_OBJ_DEF;
        }
    }
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Return(UnumInstance* c, UnumInternalObjStack* params)
{
    // Return is a special keyword
    // It affects the program execution order

    // Remember, 0 is reserved in the stack
    if(params->count < 1 || params->count > 2)
    {
        Unum_Internal_Exception(c, "@return needs to be called only with a return value (if applicable", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Return value (if applicable)
    if(params->count == 1)
        return UNUM_OBJ_SAFE;
    else
        return params->objects[1];
}

UNUM_DEF UnumInternalObject Unum_Internal_Keyword_Native(UnumInstance* c, UnumInternalObjStack* params)
{
    // Native is a special keyword
    // It calls native C code

    // Remember, 0 is reserved in the stack
    if(params->count < 2)
    {
        Unum_Internal_Exception(c, "@native needs to be called with a function name and arguments", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }
    if(params->objects[1].type != UNUM_OBJ_SYMBOL)
    {
        Unum_Internal_Exception(c, "@native expected symbol (function name) as first argument", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Lookup native function name
    // Todo - Optimize with hash table
    // Remember, 0 is reserved
    size id = 0;
    for(size i = 1; i < c->count; i++)
    {
        if(strcmp(((UnumInternalObjSymbol*)params->objects[1].data)->names[0], c->native[i].name) == 0)
        {
            id = i;
            break;
        }
    }

    // Verify function
    if(id == 0)
    {
        Unum_Internal_Exception(c, "@native could not find function with given symbol name", UNUM_CODE_RUNTIME_ERROR, 0);
        return UNUM_OBJ_DEF;
    }

    // Create stack with function (element 1) removed
    UnumInternalObjStack ns;
    ns.count = params->count - 1;
    ns.objects = UNUM_MALLOC(sizeof(UnumInternalObject) * ns.count);
    ns.objects[0] = UNUM_OBJ_DEF;
    memcpy(&ns.objects[1], &params->objects[2], sizeof(UnumInternalObject) * (ns.count - 1));

    // Link stack to parent?

    // Call function
    // Todo - Verify arguments match in type and number
    UnumInternalObject res = c->native[id].function(c, &ns);

    return res;
}

/****************************************************************
 * Language execution
 ****************************************************************/

// Todo - Finish
// Note that this function may not necessarily access values only within the given range
UNUM_DEF UnumInternalObject Unum_Internal_Execute_Data(UnumInstance* c, UnumInternalObjStack* ns, UnumInternalPair range)
{
    // Get current location
    size current = range.a;

    // Get first token
    UnumInternalToken active = c->program.tokens[range.a];

    // Cover different types of arguments

    // Handle expressions
    if(strcmp(active.value, "(") == 0)
    {
        // Create new stack
        UnumInternalObjStack* st = UNUM_MALLOC(sizeof(UnumInternalObjStack) * 1);
        st->count = 1;
        st->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);
        st->objects[0] = UNUM_OBJ_DEF;

        ns->objects[0].name = SAFE;
        ns->objects[0].type = UNUM_OBJ_STACK;
        ns->objects[0].data = st;

        // Recursively execute expressions
        return Unum_Internal_Execute_Expressions(c, st, (UnumInternalPair){.a = range.a + 1, .b = range.b - 1});

        // Handle sets
    } else if(strcmp(active.value, "{") == 0)
    {
        // Create set object
        UnumInternalObjSet* set = UNUM_MALLOC(sizeof(UnumInternalObjSet) * 1);

        // Get set data
        UnumInternalPair set_range;
        if(range.b - range.a <= 1)
            set_range = range;
        else
            set_range = (UnumInternalPair) {.a = range.a + 1, range.b - 1};

        UnumInternalPairList set_data = Unum_Internal_Parse_Separate(c->program, set_range, ",", UNUM_TRUE);
        unum_log("Set element(s):");
        Unum_Debug_Pair(c->program, set_data);

        // Verify valid arguments
        if(set_data.pairs == NULL)
        {
            Unum_Internal_Exception(c, "Invalid syntax (set)", UNUM_CODE_SYNTAX_ERROR, 0);
            UNUM_FREE(set);
            UNUM_FREE(set_data.pairs);
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
        }

        // Set set count
        set->count = set_data.num;

        // Handle empty set
        if(set_data.num == 0)
        {
            set->keys = SAFE;
            set->values = SAFE;
        } else {
            set->keys = UNUM_MALLOC(sizeof(str) * set_data.num);
            set->values = UNUM_MALLOC(sizeof(UnumInternalObject) * set_data.num);
        }

        // Iterate over key/pair values
        for(size sc = 0; sc < set_data.num; sc++)
        {
            // Get key/pair value of element in set
            UnumInternalPairList tmp_args = Unum_Internal_Parse_Separate(c->program, set_data.pairs[sc], ":", UNUM_TRUE);

            unum_log("Set element:");
            Unum_Debug_Pair(c->program, tmp_args);

            // Verify valid set (must contain a key/pair value, 2 elements total)
            if(tmp_args.pairs == NULL || tmp_args.num != 2)
            {
                Unum_Internal_Exception(c, "Invalid syntax (set - key/pair)", UNUM_CODE_SYNTAX_ERROR, 0);
                UNUM_FREE(set_data.pairs);
                return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
            }

            // Verify first element is syntactically valid
            if(tmp_args.pairs[0].a != tmp_args.pairs[0].b || c->program.tokens[tmp_args.pairs[0].a].type != UNUM_LANG_SYMBOL)
            {
                Unum_Internal_Exception(c, "Invalid syntax (set - bad key)", UNUM_CODE_SYNTAX_ERROR, 0);
                UNUM_FREE(set_data.pairs);
                UNUM_FREE(tmp_args.pairs);
                return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
            }

            // Get first element
            // Should be a key and therefore a single symbol value
            // Idea - Allow for strings in the future
            set->keys[sc] = Unum_Internal_Utility_Strdup(c->program.tokens[tmp_args.pairs[0].a].value);

            // Get second element
            UnumInternalObject pair_value = Unum_Internal_Execute_Data(c, ns, tmp_args.pairs[1]);

            // Verify object exists
            if(Unum_Internal_Object_Null(pair_value) || Unum_Internal_Object_Safe(pair_value))
            {
                Unum_Internal_Exception(c, "Object desn't exist (set)", UNUM_CODE_RUNTIME_ERROR, 0);
                return UNUM_OBJ_DEF;
            } else {
                set->values[sc] = pair_value;
            }
        }

        // Create object
        UnumInternalObject obj;
        obj.name = SAFE;
        obj.data = set;
        obj.type = UNUM_OBJ_SET;

        // Return object
        return obj;

        // Handle arrays
    } else if(strcmp(active.value, "[") == 0)
    {
        // Create array object
        UnumInternalObjArray* arr = UNUM_MALLOC(sizeof(UnumInternalObjArray) * 1);

        // Get array data
        UnumInternalPairList arr_data = Unum_Internal_Parse_Separate(c->program, range, ",", UNUM_FALSE);

        // Verify valid arguments
        if(arr_data.pairs == NULL)
        {
            Unum_Internal_Exception(c, "Invalid syntax (array - values)", UNUM_CODE_SYNTAX_ERROR, 0);
            UNUM_FREE(arr);
            UNUM_FREE(arr_data.pairs);
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
        }

        // Set set count
        arr->count = arr_data.num;

        // Handle empty array
        if(arr_data.num == 0)
        {
            // Set defaults
            arr->data = SAFE;
            arr->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_NULL];

            // Create object
            UnumInternalObject obj;
            obj.name = SAFE;
            obj.data = arr;
            obj.type = UNUM_OBJ_ARRAY;

            // Return object
            return obj;
        }

        // Array is not empty

        // Get first element to use to compare types
        UnumInternalObject first = Unum_Internal_Execute_Data(c, ns, arr_data.pairs[0]);

        // Verify object
        if(Unum_Internal_Object_Null(first))
        {
            Unum_Internal_Exception(c, "Object doesn't exist (array)", UNUM_CODE_RUNTIME_ERROR, 0);
            UNUM_FREE(arr);
            UNUM_FREE(arr_data.pairs);
            return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Invalid syntax", current, UNUM_CODE_SYNTAX_ERROR);
        }

        // Todo - Accurately set this
        arr->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_NULL];

        // Set array pointer
        arr->data = UNUM_MALLOC(sizeof(UnumInternalObject) * arr_data.num);

        // Iterate over elements
        for(size ac = 0; ac < arr_data.num; ac++)
        {
            // Get element at index
            UnumInternalObject next = Unum_Internal_Execute_Data(c, ns, arr_data.pairs[ac]);

            // Todo - Compare types (current function is unusable)
            /*
            if(!Unum_Internal_Execute_Same_Type(first, next))
            {
                UNUM_FREE(arr);
                UNUM_FREE(arr_data.pairs);
                return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Array elements don't have same type", current, UNUM_CODE_SYNTAX_ERROR);
            }
            */

            // Add object to array
            arr->data[ac] = next;
        }

        // Create object
        UnumInternalObject obj;
        obj.name = SAFE;
        obj.data = arr;
        obj.type = UNUM_OBJ_ARRAY;

        // Return object
        return obj;

        // Handle objects/symbols
        // This section covers both symbols as a link to an object and as a literal value
        // Remember, function calls are handled by the Execute_Expressions function
    } else if(active.type == UNUM_LANG_SYMBOL)
    {
        // Lookup object
        UnumInternalObject obj;
        UnumInternalPair idx = Unum_Internal_Execute_Id(c, active.value);
        unum_log("Looked-up \"%s\", got %u-%u", active.value, idx.a, idx.b);

        // Check if object doesn't exist
        if(idx.b == 0)
        {
            // As the object doesn't exist, assume this is meant to be a symbol literal (a "word")
            // Todo - Use common code for this and the UNUM_LANG_WORD section

            // Create symbol object
            UnumInternalObjSymbol* val = UNUM_MALLOC(sizeof(UnumInternalObjSymbol) * 1);
            val->count = 1;
            val->names = UNUM_MALLOC(sizeof(str*) * val->count);

            // Check for member access
            if(range.b < c->program.count && strcmp(c->program.tokens[range.a + 1].value, ".") == 0)
            {
                bool expect_symbol = UNUM_FALSE;
                size last = range.a + 1;

                // Seek to end of member access chain
                while(last < c->program.count)
                {
                    // Check for operator/symbol
                    if(expect_symbol)
                    {
                        if(c->program.tokens[last].type != UNUM_LANG_SYMBOL ||
                                c->program.tokens[last].type != UNUM_LANG_WORD)
                        {
                            last = range.a;
                            break;
                        }

                        // Add next symbol
                        val->count++;
                        val->names = UNUM_REALLOC(val->names, sizeof(str*) * val->count);

                        // Check if symbol
                        if(c->program.tokens[last].type == UNUM_LANG_SYMBOL)
                            // Copy full string
                            val->names[val->count - 1] = Unum_Internal_Utility_Strdup(c->program.tokens[last].value);
                        else
                            // Skip first character (symbolize operator)
                            val->names[val->count - 1] = Unum_Internal_Utility_Strdup(c->program.tokens[last].value + (sizeof(txt) + 1));
                    } else {
                        if(strcmp(c->program.tokens[last].value, ".") != 0)
                        {
                            // End of chain
                            // last = range.a;
                            break;
                        }
                    }

                    expect_symbol = !expect_symbol;
                    last++;
                }

                // Syntax error occurred
                if(last == range.a)
                {
                    Unum_Internal_Exception(c, "Invalid syntax (symbol - word)", UNUM_CODE_SYNTAX_ERROR, range.a);
                    UNUM_FREE(val->names);
                    return UNUM_OBJ_DEF;
                }
            } else {
                // Create symbol from token
                val->names[0] = Unum_Internal_Utility_Strdup(active.value);
            }

            // Set object properties
            // Copy first symbol as name
            obj.name = Unum_Internal_Utility_Strdup(val->names[0]);
            obj.data = val;
            obj.type = UNUM_OBJ_SYMBOL;
        } else {
            // Symbol refers to an object

            // Get object
            obj = Unum_Internal_Execute_Level(c, idx.a)->objects[idx.b];

            // Check for member access
            if(range.a < c->program.count - 1 && strcmp(c->program.tokens[range.a + 1].value, ".") == 0)
            {
                bool expect_symbol = UNUM_FALSE;
                size last = range.a + 1;

                // Seek to end of member access chain
                while(last < c->program.count)
                {
                    // Check for operator/symbol
                    if(expect_symbol)
                    {
                        if(c->program.tokens[last].type != UNUM_LANG_SYMBOL)
                        {
                            last = range.a;
                            break;
                        }

                        // Verify next member
                        if(obj.data == NULL)
                        {
                            last = range.a;
                            break;
                        }

                        // Get next member
                        if(obj.type == UNUM_OBJ_SET)
                        {
                            UnumInternalObjSet* set = ((UnumInternalObjSet*)obj.data);

                            // Todo - Optimize with hash map
                            bool found = UNUM_FALSE;
                            for(size si = 0; si < set->count; si++)
                            {
                                if(strcmp(set->keys[si], c->program.tokens[last].value) == 0)
                                {
                                    found = UNUM_TRUE;
                                    obj = set->values[si];
                                    break;
                                }
                            }

                            // Member name not found
                            if(!found)
                            {
                                last = range.a;
                                break;
                            }

                        } else if(obj.type == UNUM_OBJ_ARRAY) {
                            // Idea - Allow for access in the future via member operator
                            // For now return an error
                            Unum_Internal_Exception(c, "Internal - not implemented (array)", UNUM_CODE_SYNTAX_ERROR, range.a);
                            last = range.a;
                            break;
                        } else if(obj.type == UNUM_OBJ_NAMESPACE) {
                            UnumInternalObjNamespace* nspace = ((UnumInternalObjNamespace*)obj.data);

                            // Todo - Optimize with hash map
                            bool found = UNUM_FALSE;
                            for(size ni = 0; ni < nspace->stack.count; ni++)
                            {
                                if(Unum_Internal_Valid(nspace->stack.objects[ni].name))
                                {
                                    if(strcmp(nspace->stack.objects[ni].name, c->program.tokens[last].value) == 0)
                                    {
                                        found = UNUM_TRUE;
                                        obj = nspace->stack.objects[ni];
                                        break;
                                    }
                                }
                            }

                            // Member name not found
                            if(!found)
                            {
                                Unum_Internal_Exception(c, "Member not found (namespace)", UNUM_CODE_RUNTIME_ERROR, range.a);
                                last = range.a;
                                break;
                            }
                        } else if(obj.type == UNUM_OBJ_STRUCTURE) {
                            // Todo - Implement
                            // For now return an error
                            Unum_Internal_Exception(c, "Internal - not implemented (structure)", UNUM_CODE_SYNTAX_ERROR, range.a);
                            last = range.a;
                            break;
                        } else {
                            // Can't use member access operator here
                            Unum_Internal_Exception(c, "Cannot use member access operator on object", UNUM_CODE_SYNTAX_ERROR, range.a);
                            last = range.a;
                            break;
                        }
                    } else {
                        if(strcmp(c->program.tokens[last].value, ".") != 0)
                        {
                            // End of chain
                            // last = range.a;
                            break;
                        }
                    }

                    expect_symbol = !expect_symbol;
                    last++;
                }

                // Syntax error occurred
                if(last == range.a)
                {
                    Unum_Internal_Exception(c, "Invalid syntax (symbol - object)", UNUM_CODE_SYNTAX_ERROR, range.a);
                    return UNUM_OBJ_DEF;
                }
            }
        }

        // Return object
        return obj;

        // Handle symbol constants
    } else if(active.type == UNUM_LANG_WORD) {
        // Create symbol object
        UnumInternalObjSymbol* val = UNUM_MALLOC(sizeof(UnumInternalObjSymbol) * 1);
        val->count = 1;
        val->names = UNUM_MALLOC(sizeof(str*) * val->count);

        // Check for member access
        if(strcmp(c->program.tokens[range.a + 1].value, ".") == 0)
        {
            bool expect_symbol = UNUM_FALSE;
            size last = range.a + 1;

            // Seek to end of member access chain
            while(last < c->program.count)
            {
                // Check for operator/symbol
                if(expect_symbol)
                {
                    if(c->program.tokens[last].type != UNUM_LANG_SYMBOL ||
                            c->program.tokens[last].type != UNUM_LANG_WORD)
                    {
                        last = range.a;
                        break;
                    }

                    // Add next symbol
                    val->count++;
                    val->names = UNUM_REALLOC(val->names, sizeof(str*) * val->count);

                    // Check if symbol
                    if(c->program.tokens[last].type == UNUM_LANG_SYMBOL)
                        // Copy full string
                        val->names[val->count - 1] = Unum_Internal_Utility_Strdup(c->program.tokens[last].value);
                    else
                        // Skip first character (symbolize operator)
                        val->names[val->count - 1] = Unum_Internal_Utility_Strdup(c->program.tokens[last].value + (sizeof(txt) + 1));
                } else {
                    if(strcmp(c->program.tokens[last].value, ".") != 0)
                    {
                        // End of chain
                        // last = range.a;
                        break;
                    }
                }

                expect_symbol = !expect_symbol;
                last++;
            }

            // Syntax error occurred
            if(last == range.a)
            {
                Unum_Internal_Exception(c, "Invalid syntax (word)", UNUM_CODE_SYNTAX_ERROR, 0);
                UNUM_FREE(val->names);
                return UNUM_OBJ_DEF;
            }
        } else {
            // Create symbol from token (with first character removed)
            val->names[0] = Unum_Internal_Utility_Strdup(&active.value[1]);
        }

        // Create object
        // Copy first symbol as name
        UnumInternalObject obj;
        obj.name = Unum_Internal_Utility_Strdup(val->names[0]);
        obj.data = val;
        obj.type = UNUM_OBJ_SYMBOL;

        // Return object
        return obj;

        // Handle char constants
    } else if(active.type == UNUM_LANG_CHAR) {
        // Assumes char is ASCII (unsigned 8 bit characters)
        // Todo - Process char to handle escape sequences
        UnumInternalObjSingle* val = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);
        val->data = UNUM_MALLOC(sizeof(u8) * 1);
        val->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_U8];
        ((u8*)val->data)[0] = (u8)(active.value[1]); // Copy first character inside quotes, ignore rest (for now)

        // Create object
        UnumInternalObject obj;
        obj.name = SAFE;
        obj.data = val;
        obj.type = UNUM_OBJ_SINGLE;

        // Return object
        return obj;

        // Handle string constants
        // Todo - Extend the UNUM_OBJ_DATA structure to all primitive array types
    } else if(active.type == UNUM_LANG_STRING) {
        // Assumes string is ASCII (unsigned 8 bit characters)
        UnumInternalObjArray* arr = UNUM_MALLOC(sizeof(UnumInternalObjArray) * 1);
        arr->count = (size)strlen(active.value);
        arr->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_U8];

        // Create data object
        UnumInternalObject o;
        o.name = SAFE;
        o.data = Unum_Internal_Utility_Strdup(active.value);
        o.type = UNUM_OBJ_DATA;

        // Add data object to array
        arr->data = UNUM_MALLOC(sizeof(UnumInternalObject) * 1);
        arr->data[0] = o;

        // Todo - Process string to handle escape sequences
        // Todo - Make keyword table and lookup function arguments/names from there

        // Create object
        UnumInternalObject obj;
        obj.name = SAFE;
        obj.data = arr;
        obj.type = UNUM_OBJ_ARRAY;

        // Return object
        return obj;

        // Handle numeric constants
    } else if(active.type == UNUM_LANG_NUMBER) {
        // Create number object
        UnumInternalObjSingle* dat = UNUM_MALLOC(sizeof(UnumInternalObjSingle) * 1);

        // Check if number is floating point
        // Assumes number is in base 10 format
        if(strchr(active.value, '.') != NULL)
        {
            // Use default type f64
            f64* v = UNUM_MALLOC(sizeof(f64) * 1);
            *v = strtod(active.value, NULL);
            dat->data = v;
            dat->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_F64];
        } else {
            // Check if negative
            if(strchr(active.value, '-') != NULL)
            {
                // Use default type i64
                i64* v = UNUM_MALLOC(sizeof(i64) * 1);
                *v = strtol(active.value, NULL, 10);
                dat->data = v;
                dat->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_I64];
            } else {
                // Use default type u64
                u64* v = UNUM_MALLOC(sizeof(u64) * 1);
                *v = strtoul(active.value, NULL, 10);
                dat->data = v;
                dat->type = UNUM_PRIMITIVES[UNUM_PRIMITIVE_U64];
            }
        }

        // Todo - Make keyword table and lookup function arguments/names from there

        // Create object
        UnumInternalObject obj;
        obj.name = SAFE;
        obj.data = dat;
        obj.type = UNUM_OBJ_SINGLE;

        // Return object
        return obj;

    } else {
        // Todo - Handle other types of values
        Unum_Internal_Exception(c, "Invalid syntax - unknown value(s)", UNUM_CODE_SYNTAX_ERROR, current);
        return UNUM_OBJ_DEF;
    }
}

// Todo - Finish
// Return location of error on problem, otherwise return 0
UNUM_DEF UnumInternalObject Unum_Internal_Execute_Expressions(UnumInstance* c, UnumInternalObjStack* base, UnumInternalPair p)
{
    // unum_log("Code:");
    // unum_log("\"%s\"", c->program.code);
    unum_log("Execution:");
    Unum_Debug_Sequence(c->program, p);

    // Split into expressions
    UnumInternalPairList top = Unum_Internal_Parse_Separate(c->program, (UnumInternalPair){.a = p.a, .b = p.b}, ";", UNUM_TRUE); // Unum_Internal_Parse_Chain(c->program, last, &out_num);
    unum_log("Statements:");
    Unum_Debug_Pair(c->program, top);

    // Verify pairs
    if(top.pairs == NULL)
    {
        unum_log("Invalid chain");
        Unum_Internal_Exception(c, "Invalid syntax (statements)", UNUM_CODE_SYNTAX_ERROR, 0);
        return UNUM_OBJ_DEF;
    } else if(top.num == 0 && top.pairs == SAFE){
        unum_log("No statement chain");
        return UNUM_OBJ_SAFE;
    }

    // Get highest stack level
    UnumInternalObjStack* last_ns;
    if(base == NULL)
    {
        last_ns = Unum_Internal_Execute_Stack(c);
    } else {
        last_ns = base;
    }

    // Iterate over expressions
    for(size i = 0; i < top.num; i++)
    {
        // Verify that expression isn't empty
        if(top.pairs[i].a >= top.pairs[i].b)
            continue;

        // Split into sub-expressions (statements)
        UnumInternalPairList expressions = Unum_Internal_Parse_Separate(c->program, top.pairs[i], "->", UNUM_TRUE);
        unum_log("Expressions:");
        Unum_Debug_Pair(c->program, expressions);

        // Verify pairs
        if(expressions.num == 0 || expressions.pairs == NULL)
        {
            Unum_Internal_Exception(c, "Invalid syntax (expressions)", UNUM_CODE_SYNTAX_ERROR, 0);
            UNUM_FREE(top.pairs);
            return UNUM_OBJ_DEF;
        }

        // Iterate over statements
        for(size u = 0; u < expressions.num; u++)
        {
            // First token should be function/keyword name of current statement
            size current = expressions.pairs[u].a;
            UnumInternalToken tok = c->program.tokens[current];

            // Get arguments

            // Calculate range
            UnumInternalPair arg_range;
            UnumInternalPairList args;

            // Handle no argument function
            if(expressions.pairs[u].a == expressions.pairs[u].b)
            {
                arg_range = expressions.pairs[u];
                args.num = 0;
                args.pairs = SAFE;
                // Function has argument(s)
            } else {
                // Starts with function name
                arg_range.a = expressions.pairs[u].a;
                // End with pass operator/end operator
                arg_range.b = expressions.pairs[u].b; //u == expressions.num - 1 ? expressions.pairs[u].b : expressions.pairs[u + 1].a - 1;

                // Check if keyword
                if(c->program.tokens[arg_range.a].type == UNUM_LANG_KEYWORD)
                {
                    arg_range.a++;
                } else if(c->program.tokens[arg_range.a].type == UNUM_LANG_SYMBOL) {
                    // Seek to end of symbol chain
                    bool expect_symbol = UNUM_TRUE;
                    for(; arg_range.a <= arg_range.b; arg_range.a++)
                    {
                        if(expect_symbol)
                        {
                            if(c->program.tokens[arg_range.a].type != UNUM_LANG_SYMBOL)
                                break;
                        } else {
                            if(strcmp(c->program.tokens[arg_range.a].value, ".") != 0)
                                break;
                        }
                        expect_symbol = !expect_symbol;
                    }
                    // arg_range.a -= 1;
                } else {
                    unum_log("%s", c->program.tokens[arg_range.a].value);
                    Unum_Internal_Exception(c, "Expected function or keyword call", UNUM_CODE_SYNTAX_ERROR, 0);
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    return UNUM_OBJ_DEF;
                }

                if(arg_range.a <= arg_range.b)
                {
                    unum_log("Pair: \"%s\" (%u) and \"%s\" (%u)", c->program.tokens[arg_range.a].value, arg_range.a, c->program.tokens[arg_range.b].value, arg_range.b);
                    unum_log("Argument Chain:");
                    //Unum_Debug_Sequence(c->program, arg_range);
                }

                // Parse arguments
                args = Unum_Internal_Parse_Separate(c->program, arg_range, ",", UNUM_FALSE);
                unum_log("Argument(s):");
                Unum_Debug_Pair(c->program, args);
            }

            // Verify valid arguments
            if(args.pairs == NULL)
            {
                Unum_Internal_Exception(c, "Invalid syntax (arguments)", UNUM_CODE_SYNTAX_ERROR, 0);
                UNUM_FREE(top.pairs);
                UNUM_FREE(expressions.pairs);
                return UNUM_OBJ_DEF;
            }

            // Create stack for argument values
            UnumInternalObjStack* ns = UNUM_MALLOC(sizeof(UnumInternalObjStack) * 1);

            // Add latest object to current stack if pass operator was used
            // Remember, location 0 is reserved, so add 1 to the argument stack anyways
            if(u > 0)
            {
                ns->count = args.num + 2;
                ns->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * (ns->count));

                ns->objects[1] = last_ns->objects[last_ns->count - 1];

                // Remove latest object from lower stack
                last_ns->objects[last_ns->count - 1] = UNUM_OBJ_DEF;
                last_ns->count--;
            } else {
                ns->count = args.num + 1;
                ns->objects = UNUM_MALLOC(sizeof(UnumInternalObject) * (ns->count));
            }

            // Clear element at location 0
            ns->objects[0] = UNUM_OBJ_DEF;

            // Add stack to global stack
            // Necessary for proper name resolution
            last_ns->objects[0].name = SAFE;
            last_ns->objects[0].data = ns;
            last_ns->objects[0].type = UNUM_OBJ_STACK;

            // Try keyword matching
            if(tok.type == UNUM_LANG_KEYWORD)
            {
                // Todo - Optimize with hash table
                // Remember, the keyword at index 0 is reserved
                size id = 0;
                for(size v = 0; v < sizeof(UNUM_KEYWORDS) / sizeof(UnumInternalKeyword); v++)
                {
                    // Compare keywords, add 1 to skip operator "@"
                    if(strcmp(UNUM_KEYWORDS[v].name, tok.value + (1 * sizeof(txt))) == 0)
                    {
                        id = v;
                        break;
                    }
                }

                // Verify keyword exists
                if(id == 0)
                {
                    Unum_Internal_Exception(c, "Unknown/non-existent keyword", UNUM_CODE_RUNTIME_ERROR, 0);
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    if(Unum_Internal_Valid(args.pairs))
                        UNUM_FREE(args.pairs);
                    return UNUM_OBJ_DEF; // UNUM_INTERNAL_RESULT("Unknown/non-existent keyword", current, UNUM_CODE_RUNTIME_ERROR);
                }

                // Handle arguments
                // Special cases for some keywords
                // Namespace keyword
                if(id == UNUM_KEYWORD_NAMESPACE)
                {
                    // Verify arguments
                    // Must match the keyword function
                    if(args.num != 2)
                    {
                        Unum_Internal_Exception(c, "Expected arguments for @namespace", UNUM_CODE_RUNTIME_ERROR, 0);
                        UNUM_FREE(ns);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        if(Unum_Internal_Valid(args.pairs))
                            UNUM_FREE(args.pairs);
                        return UNUM_OBJ_DEF;
                    }

                    // Create argument objects

                    UnumInternalObject obj_name = Unum_Internal_Execute_Data(c, ns, args.pairs[0]);

                    UnumInternalObjExpression* range_pair = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                    range_pair->pair = args.pairs[1];

                    UnumInternalObject obj_range;
                    obj_range.name = SAFE;
                    obj_range.type = UNUM_OBJ_EXPRESSION;
                    obj_range.data = range_pair;

                    // Add arguments to stack
                    if(u > 0)
                    {
                        ns->objects[2] = obj_name;
                        ns->objects[3] = obj_range;
                    } else {
                        ns->objects[1] = obj_name;
                        ns->objects[2] = obj_range;
                    }
                    // Body keyword
                } else if(id == UNUM_KEYWORD_BODY) {
                    // Verify arguments
                    // Must match the keyword function
                    if(args.num != 1)
                    {
                        Unum_Internal_Exception(c, "Expected arguments for @body", UNUM_CODE_RUNTIME_ERROR, 0);
                        UNUM_FREE(ns);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        if(Unum_Internal_Valid(args.pairs))
                            UNUM_FREE(args.pairs);
                        return UNUM_OBJ_DEF;
                    }

                    // Create argument objects

                    UnumInternalObjExpression* range_expression = UNUM_MALLOC(sizeof(UnumInternalObjExpression) * 1);
                    range_expression->pair = args.pairs[0];

                    UnumInternalObject obj_range;
                    obj_range.name = SAFE;
                    obj_range.type = UNUM_OBJ_EXPRESSION;
                    obj_range.data = range_expression;

                    // Add arguments to stack
                    if(u > 0)
                    {
                        ns->objects[2] = obj_range;
                    } else {
                        ns->objects[1] = obj_range;
                    }
                    // Return keyword
                } else if(id == UNUM_KEYWORD_IF) {
                    // Verify and create arguments
                    // Must match the keyword function
                    // No arguments
                    if(args.num == 0)
                    {
                        // Do nothing
                        // One argument
                    } else if((u > 0 ? 1 : 0) + args.num == 1)
                    {
                        if(u == 0)
                        {
                            // Create first object
                            UnumInternalObject obj_first = Unum_Internal_Execute_Data(c, ns, args.pairs[0]);

                            // Add to stack
                            ns->objects[1] = obj_first;
                        }
                        // Two arguments
                    } else if((u > 0 ? 1 : 0) + args.num == 2)
                    {
                        if(u == 0)
                        {
                            // Create first object
                            UnumInternalObject obj_first = Unum_Internal_Execute_Data(c, ns, args.pairs[0]);

                            // Add to stack
                            ns->objects[1] = obj_first;
                        }

                        // Create expression object
                        UnumInternalObjExpression* range_pair_true = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                        range_pair_true->pair = args.pairs[1];

                        UnumInternalObject obj_range_true;
                        obj_range_true.name = SAFE;
                        obj_range_true.type = UNUM_OBJ_EXPRESSION;
                        obj_range_true.data = range_pair_true;

                        // Add to stack
                        ns->objects[2] = obj_range_true;

                        // Three arguments
                    } else if((u > 0 ? 1 : 0) + args.num == 3)
                    {
                        if(u == 0)
                        {
                            // Create first object
                            UnumInternalObject obj_first = Unum_Internal_Execute_Data(c, ns, args.pairs[0]);

                            // Add to stack
                            ns->objects[1] = obj_first;
                        }

                        // Create expression objects
                        UnumInternalObjExpression* range_pair_true = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                        range_pair_true->pair = args.pairs[1];
                        UnumInternalObjExpression* range_pair_false = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                        range_pair_false->pair = args.pairs[2];

                        UnumInternalObject obj_range_true;
                        obj_range_true.name = SAFE;
                        obj_range_true.type = UNUM_OBJ_EXPRESSION;
                        obj_range_true.data = range_pair_true;

                        UnumInternalObject obj_range_false;
                        obj_range_false.name = SAFE;
                        obj_range_false.type = UNUM_OBJ_EXPRESSION;
                        obj_range_false.data = range_pair_false;

                        // Add objects to stack
                        ns->objects[2] = obj_range_true;
                        ns->objects[3] = obj_range_false;
                    } else {
                        Unum_Internal_Exception(c, "Expected proper arguments for @if", UNUM_CODE_RUNTIME_ERROR, 0);
                        UNUM_FREE(ns);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        if(Unum_Internal_Valid(args.pairs))
                            UNUM_FREE(args.pairs);
                        return UNUM_OBJ_DEF;
                    }
                    // Return keyword
                } else if(id == UNUM_KEYWORD_LOOP) {
                    // Verify and create arguments
                    // Must match the keyword function
                    // Two arguments
                    if((u > 0 ? 1 : 0) + args.num == 2)
                    {
                        if(u == 0)
                        {
                            // Create first object
                            UnumInternalObject obj_first = Unum_Internal_Execute_Data(c, ns, args.pairs[0]);

                            // Add to stack
                            ns->objects[1] = obj_first;
                        }

                        // Create expression object
                        UnumInternalObjExpression* range_pair = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                        range_pair->pair = args.pairs[1];

                        UnumInternalObject obj_range;
                        obj_range.name = SAFE;
                        obj_range.type = UNUM_OBJ_EXPRESSION;
                        obj_range.data = range_pair;

                        // Add object to stack
                        ns->objects[2] = obj_range;
                        // Four arguments
                    } else if((u > 0 ? 1 : 0) + args.num == 4)
                    {
                        // As the first object is an expression, it must be created differently
                        if(u == 0)
                        {
                            // Create first object
                            UnumInternalObjExpression* range_pair_initial = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                            range_pair_initial->pair = args.pairs[0];

                            UnumInternalObject obj_range_initial;
                            obj_range_initial.name = SAFE;
                            obj_range_initial.type = UNUM_OBJ_EXPRESSION;
                            obj_range_initial.data = range_pair_initial;

                            // Add to stack
                            ns->objects[1] = obj_range_initial;
                        }

                        // Create expression objects
                        UnumInternalObjExpression* range_pair_condition = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                        range_pair_condition->pair = args.pairs[1];
                        UnumInternalObjExpression* range_pair_increment = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                        range_pair_increment->pair = args.pairs[2];
                        UnumInternalObjExpression* range_pair_main = UNUM_MALLOC(sizeof(UnumInternalPair) * 1);
                        range_pair_main->pair = args.pairs[3];

                        UnumInternalObject obj_range_condition;
                        obj_range_condition.name = SAFE;
                        obj_range_condition.type = UNUM_OBJ_EXPRESSION;
                        obj_range_condition.data = range_pair_condition;

                        UnumInternalObject obj_range_increment;
                        obj_range_increment.name = SAFE;
                        obj_range_increment.type = UNUM_OBJ_EXPRESSION;
                        obj_range_increment.data = range_pair_increment;

                        UnumInternalObject obj_range_main;
                        obj_range_main.name = SAFE;
                        obj_range_main.type = UNUM_OBJ_EXPRESSION;
                        obj_range_main.data = range_pair_main;

                        // Add objects to stack
                        ns->objects[2] = obj_range_condition;
                        ns->objects[3] = obj_range_increment;
                        ns->objects[4] = obj_range_main;
                    } else {
                        Unum_Internal_Exception(c, "Expected proper arguments for @loop", UNUM_CODE_RUNTIME_ERROR, 0);
                        UNUM_FREE(ns);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        if(Unum_Internal_Valid(args.pairs))
                            UNUM_FREE(args.pairs);
                        return UNUM_OBJ_DEF;
                    }
                    // Return keyword
                } else if(id == UNUM_KEYWORD_RETURN) {
                    // Verify arguments
                    // Must match the keyword function
                    // Check if the total number of arguments (one from pass and any given) exceeds one
                    if((u > 0 ? 1 : 0) + args.num > 1)
                    {
                        Unum_Internal_Exception(c, "Expected either no arguments or one argument for @return", UNUM_CODE_SYNTAX_ERROR, 0);
                        UNUM_FREE(ns);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        if(Unum_Internal_Valid(args.pairs))
                            UNUM_FREE(args.pairs);
                        return UNUM_OBJ_DEF;
                    }

                    // Handle arguments
                    if(args.num > 0)
                    {
                        // Function has an argument

                        // Create argument object
                        UnumInternalObject obj_return = Unum_Internal_Execute_Data(c, ns, args.pairs[0]);

                        // Return object
                        UNUM_FREE(ns);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        if(Unum_Internal_Valid(args.pairs))
                            UNUM_FREE(args.pairs);
                        return obj_return;
                    } else {
                        // Return value from stack
                        if(u > 0)
                        {
                            UNUM_FREE(ns);
                            UNUM_FREE(top.pairs);
                            UNUM_FREE(expressions.pairs);
                            if(Unum_Internal_Valid(args.pairs))
                                UNUM_FREE(args.pairs);
                            return ns->objects[1];
                            // Return safe value
                        } else {
                            UNUM_FREE(ns);
                            UNUM_FREE(top.pairs);
                            UNUM_FREE(expressions.pairs);
                            if(Unum_Internal_Valid(args.pairs))
                                UNUM_FREE(args.pairs);
                            return UNUM_OBJ_SAFE;
                        }
                    }
                } else {
                    // Evaluate arguments
                    for(size e = 0; e < args.num; e++)
                    {
                        // Handle data
                        UnumInternalObject oe = Unum_Internal_Execute_Data(c, ns, args.pairs[e]);
                        // unum_log("\n---------------------------\nDATA: \"%s\" (%u) and \"%s\" (%u)\n---------------------------", c->program.tokens[args.pairs[e].a].value, args.pairs[e].a, c->program.tokens[args.pairs[e].b].value, args.pairs[e].b);

                        // Verify object
                        if(Unum_Internal_Object_Null(oe))
                        {
                            UNUM_FREE(ns);
                            UNUM_FREE(top.pairs);
                            UNUM_FREE(expressions.pairs);
                            if(Unum_Internal_Valid(args.pairs))
                                UNUM_FREE(args.pairs);
                            return UNUM_OBJ_DEF;
                        }

                        // Add object if not empty
                        if(!Unum_Internal_Object_Safe(oe))
                        {
                            // Remember to skip passed object if necessary
                            if(u > 0)
                                ns->objects[e + 2] = oe;
                            else
                                ns->objects[e + 1] = oe;
                        }
                    }
                }

                // Call keyword
                unum_log("Keyword call: \"%s\"", UNUM_KEYWORDS[id].name);
                UnumInternalObject rs = UNUM_KEYWORDS[id].func(c, ns);

                // Verify successful call
                if(Unum_Internal_Object_Null(rs)) // rs.code != UNUM_CODE_NEUTRAL)
                {
                    UNUM_FREE(ns);
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    if(Unum_Internal_Valid(args.pairs))
                        UNUM_FREE(args.pairs);
                    return UNUM_OBJ_DEF; // rs;
                }

                /*
                // Copy modified stack values back to source
                // Todo - Minimize unnecessary copying by using hashes to compare data

                // Copy passed argument
                if(u > 0)
                    last_ns->objects[last_ns->count - 1] = ns->objects[1];

                // Start value depends on whether there was a pass
                for(size si = u > 0 ? 2 : 1; si < ns->count; si++)
                {
                    // Todo - Check if values are constant

                    // Check if object exists on a lower level stack
                    UnumInternalPair obj_stack_id = Unum_Internal_Execute_Id(c, ns->objects[si].name);

                    // Object is not on any stack (might be technically unnecessary)
                    if(obj_stack_id.b == 0)
                        continue;
                    // Object is on lower level, meaning modifying its value is meaningful
                    if(obj_stack_id.a < Unum_Internal_Execute_Stack_Level(c))
                        Unum_Internal_Execute_Level(c, obj_stack_id.a)->objects[obj_stack_id.b] = ns->objects[si];
                }
                */

                // Add to current level stack if not empty object
                if(!Unum_Internal_Object_Safe(rs))
                {
                    last_ns->objects = UNUM_REALLOC(last_ns->objects, sizeof(UnumInternalObject) * (last_ns->count + 1));
                    last_ns->objects[last_ns->count] = rs;
                    last_ns->count++;
                }

                // Try function/object matching
            } else if(tok.type == UNUM_LANG_SYMBOL) {
                // Verify it is a function
                // Remember, the 1st element of the stack (index 0) is reserved
                UnumInternalObject res_obj = Unum_Internal_Execute_Data(c, ns, (UnumInternalPair) {.a = expressions.pairs[u].a, .b = arg_range.a });  // Unum_Internal_Execute_Level(c, res_id.a)->objects[res_id.b];
                if(res_obj.type == UNUM_OBJ_FUNCTION)
                {
                    // Get function object
                    UnumInternalObjFunction func_obj = *((UnumInternalObjFunction*)res_obj.data);

                    // Verify number of arguments match
                    if((u > 0 ? 1 : 0) + args.num != func_obj.parameters.count)
                    {
                        Unum_Internal_Exception(c, "Argument count doesn't match parameter count", UNUM_CODE_SYNTAX_ERROR, 0);
                        UNUM_FREE(ns);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        UNUM_FREE(args.pairs);
                        return UNUM_OBJ_DEF;
                    }

                    // Evaluate and add arguments
                    for(size e = 0; e < args.num; e++)
                    {
                        // Handle data
                        UnumInternalObject oe = Unum_Internal_Execute_Data(c, ns, args.pairs[e]);
                        if(Unum_Internal_Valid(oe.name))
                        {
                            unum_log("VAR %s", oe.name);
                        }

                        // DEBUG
                        /*
                        if(oe.type == UNUM_OBJ_SINGLE)
                        {
                            unum_log("VAR %f", *((f64*)((UnumInternalObjSingle*)oe.data)->data));
                        }
                        */

                        // Todo - Properly verify argument types match
                        if(UNUM_FALSE) // Not accurate - func_obj.parameters.values[si].type != ns->objects[si + 1].type)
                        {
                            Unum_Internal_Exception(c, "Arguments don't match function parameters", UNUM_CODE_RUNTIME_ERROR, 0);
                            UNUM_FREE(ns);
                            UNUM_FREE(top.pairs);
                            UNUM_FREE(expressions.pairs);
                            if(Unum_Internal_Valid(args.pairs))
                                UNUM_FREE(args.pairs);
                            return UNUM_OBJ_DEF;
                        }

                        // Verify object
                        if(Unum_Internal_Object_Null(oe))
                        {
                            Unum_Internal_Exception(c, "Function argument was null", UNUM_CODE_RUNTIME_ERROR, 0);
                            UNUM_FREE(ns);
                            UNUM_FREE(top.pairs);
                            UNUM_FREE(expressions.pairs);
                            if(Unum_Internal_Valid(args.pairs))
                                UNUM_FREE(args.pairs);
                            return UNUM_OBJ_DEF;
                        }

                        // Add object if not empty
                        if(!Unum_Internal_Object_Safe(oe))
                        {
                            // Remember to skip reserved object (and passed object if necessary)
                            if(u > 0)
                                ns->objects[e + 2] = oe;
                            else
                                ns->objects[e + 1] = oe;
                        } else {
                            Unum_Internal_Exception(c, "Function argument was empty", UNUM_CODE_RUNTIME_ERROR, 0);
                            UNUM_FREE(ns);
                            UNUM_FREE(top.pairs);
                            UNUM_FREE(expressions.pairs);
                            if(Unum_Internal_Valid(args.pairs))
                                UNUM_FREE(args.pairs);
                            return UNUM_OBJ_DEF;
                        }
                    }

                    // Rename stack objects
                    for(size so = 1; so < ns->count; so++)
                    {
                        // Rename object according to index
                        ns->objects[so].name = Unum_Internal_Utility_Strdup(func_obj.parameters.keys[so - 1]);
                    }

                    // Call function
                    unum_log("Calling function \"%s\"", res_obj.name);
                    UnumInternalObject func_res = Unum_Internal_Execute_Expressions(c, (UnumInternalObjStack*) last_ns->objects[0].data, func_obj.body.pair);

                    //unum_log("DATA %f", *((f64*)((UnumInternalObjSingle*)func_res.data)->data));

                    // Verify successful call
                    if(Unum_Internal_Object_Null(func_res))
                    {
                        UNUM_FREE(ns);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        if(Unum_Internal_Valid(args.pairs))
                            UNUM_FREE(args.pairs);
                        return UNUM_OBJ_DEF;
                    }

                    // Add to current level stack if not empty object
                    if(!Unum_Internal_Object_Safe(func_res))
                    {
                        last_ns->objects = UNUM_REALLOC(last_ns->objects, sizeof(UnumInternalObject) * (last_ns->count + 1));
                        last_ns->objects[last_ns->count] = func_res;
                        last_ns->count++;
                    }
                } else if(res_obj.type == UNUM_OBJ_SINGLE)
                {
                    if((u > 0 ? 1 : 0) + args.num == 1)
                    {
                        if(u == 0)
                        {
                            UnumInternalObject oe = Unum_Internal_Execute_Data(c, ns, args.pairs[0]);

                            // Verify object
                            if(Unum_Internal_Object_Null(oe))
                            {
                                Unum_Internal_Exception(c, "Function argument was null", UNUM_CODE_RUNTIME_ERROR, 0);
                                UNUM_FREE(ns);
                                UNUM_FREE(top.pairs);
                                UNUM_FREE(expressions.pairs);
                                if(Unum_Internal_Valid(args.pairs))
                                    UNUM_FREE(args.pairs);
                                return UNUM_OBJ_DEF;
                            }

                            ns->objects[1] = oe;
                        }

                        // Lookup object location
                        UnumInternalPair ol = Unum_Internal_Execute_Id(c, res_obj.name);

                        if(ol.b == 0)
                        {
                            Unum_Internal_Exception(c, "Unknown object", UNUM_CODE_RUNTIME_ERROR, 0);
                            UNUM_FREE(top.pairs);
                            UNUM_FREE(expressions.pairs);
                            if(Unum_Internal_Valid(args.pairs))
                                UNUM_FREE(args.pairs);
                            return UNUM_OBJ_DEF;
                        }

                        // Todo - Properly verify incoming and current types match
                        if(Unum_Internal_Execute_Level(c, ol.a)->objects[ol.b].type == ns->objects[1].type)
                        {
                            // Copy data
                            Unum_Internal_Execute_Level(c, ol.a)->objects[ol.b].data = ns->objects[1].data;

                            // Add to current level stack
                            if(!Unum_Internal_Object_Safe(ns->objects[1]))
                            {
                                last_ns->objects = UNUM_REALLOC(last_ns->objects, sizeof(UnumInternalObject) * (last_ns->count + 1));
                                last_ns->objects[last_ns->count] = ns->objects[1];
                                last_ns->count++;
                            }
                        } else {
                            Unum_Internal_Exception(c, "Object types don't match (single)", UNUM_CODE_RUNTIME_ERROR, 0);
                            UNUM_FREE(top.pairs);
                            UNUM_FREE(expressions.pairs);
                            if(Unum_Internal_Valid(args.pairs))
                                UNUM_FREE(args.pairs);
                            return UNUM_OBJ_DEF;
                        }
                    } else {
                        Unum_Internal_Exception(c, "Single object expected single value", UNUM_CODE_RUNTIME_ERROR, 0);
                        UNUM_FREE(top.pairs);
                        UNUM_FREE(expressions.pairs);
                        if(Unum_Internal_Valid(args.pairs))
                            UNUM_FREE(args.pairs);
                        return UNUM_OBJ_DEF;
                    }
                } else {
                    // Handle other object types later
                    // For now throw an error
                    Unum_Internal_Exception(c, "Expected function/object call", UNUM_CODE_RUNTIME_ERROR, 0);
                    UNUM_FREE(top.pairs);
                    UNUM_FREE(expressions.pairs);
                    if(Unum_Internal_Valid(args.pairs))
                        UNUM_FREE(args.pairs);
                    return UNUM_OBJ_DEF;
                }
                // Bad syntax
            } else {
                Unum_Internal_Exception(c, "Expected symbol/keyword for call", UNUM_CODE_RUNTIME_ERROR, 0);
                UNUM_FREE(top.pairs);
                UNUM_FREE(expressions.pairs);
                if(Unum_Internal_Valid(args.pairs))
                    UNUM_FREE(args.pairs);
                return UNUM_OBJ_DEF;
            }

            // Delete stack
            UNUM_FREE(ns);
            last_ns->objects[0] = UNUM_OBJ_DEF;
        }

        // Free expression ranges
        UNUM_FREE(expressions.pairs);
    }

    // Free overall expression ranges
    UNUM_FREE(top.pairs);

    // Return object on the top of the stack if not empty
    if(last_ns->count > 1)
    {
        return last_ns->objects[last_ns->count - 1];
    } else {
        // Otherwise return empty object
        return UNUM_OBJ_SAFE;
    }
}

/****************************************************************
 * Front end
 ****************************************************************/

// Unum standard libraries
#include "libraries/UMath.h"
#include "libraries/UIO.h"
#include "libraries/UGlobal.h"

UNUM_DEF i32 Unum_Execute(UnumInstance* c, str code)
{
    // Todo - Allow for calling this function multiple times
    c->code = UNUM_REALLOC(c->code, sizeof(txt) * (strlen(c->code) + strlen(code)));
    strcat(c->code, code);

    c->program = Unum_Internal_Tokenize(c->code);

    // unum_log("Code:\n\t%s\n", c->code);
    UnumInternalObject ob = Unum_Internal_Execute_Expressions(c, &c->stack, (UnumInternalPair){.a = 0, .b = c->program.count - 1});

    if(!Unum_Internal_Object_Null(ob))
    {
        c->stack.objects = UNUM_REALLOC(c->stack.objects, sizeof(UnumInternalObject) * (c->stack.count + 1));
        c->stack.objects[c->stack.count] = ob;
        c->stack.count++;

        Unum_Internal_Exception(c, "Ok", UNUM_CODE_NEUTRAL, 0);
    } else {
        if(c->result.message == NULL)
        {
            Unum_Internal_Exception(c, "Unknown error", UNUM_CODE_RUNTIME_ERROR, 0);
        }
    }

    return c->result.result;
}

UNUM_DEF UnumInstance* Unum_Initialize(void)
{
#ifdef UNUM_EXT
    p_libsys_init();
#endif

    UnumInstance* res = UNUM_MALLOC(sizeof(UnumInstance));

    // Create default result
    res->result.code = UNUM_CODE_NEUTRAL;
    res->result.result = 0;
    res->result.status = UNUM_CODES[UNUM_CODE_NEUTRAL].level;
    res->result.message = NULL;
    res->result.location = 0;

    // Create stack
    res->stack.count = (sizeof(UNUM_OBJECT_PRIMITIVES) / sizeof(UnumInternalObject)) + 1;
    res->stack.objects = UNUM_MALLOC(sizeof(UnumInternalObject) * res->stack.count);

    // First object is reserved
    res->stack.objects[0] = UNUM_OBJ_DEF;

    // Load primitives (skip first object)
    memcpy(&res->stack.objects[1], UNUM_OBJECT_PRIMITIVES, sizeof(UNUM_OBJECT_PRIMITIVES));

    unum_log("Stack (%lu objects):", sizeof(UNUM_OBJECT_PRIMITIVES) / sizeof(UnumInternalObject));
    for(size ee = 0; ee < res->stack.count; ee++)
    {
        unum_log("%s", res->stack.objects[ee].name);
    }

    // Library loading

    res->count = 1;

    // Allocate native function tables
    res->count += sizeof(UNUM_NATIVE_MATH) / sizeof(UnumInternalNative);
    res->count += sizeof(UNUM_NATIVE_IO) / sizeof(UnumInternalNative);
    res->count += sizeof(UNUM_NATIVE_GLOBAL) / sizeof(UnumInternalNative);

    res->native = UNUM_MALLOC(sizeof(UnumInternalNative) * res->count);

    // Load native libraries
    // Todo - Load each individually to better describe errors
    if(!Unum_Lib_Math_Load() || !Unum_Lib_Io_Load() || !Unum_Lib_Global_Load())
    {
        unum_log("Failed to load native libraries");

        // Return error
        UNUM_FREE(res->stack.objects);
        UNUM_FREE(res->native);
        UNUM_FREE(res);
        return NULL;
    } else {
        // First element is reserved
        res->native[0] = (UnumInternalNative) {.name = NULL, .function = NULL, .params = NULL, .result = UNUM_OBJ_DEF};
        // Copy libraries
        size inc = 1;
        memcpy(&res->native[inc], UNUM_NATIVE_MATH, sizeof(UNUM_NATIVE_MATH));
        inc += (sizeof(UNUM_NATIVE_MATH) / sizeof(UnumInternalNative));
        memcpy(&res->native[inc], UNUM_NATIVE_IO, sizeof(UNUM_NATIVE_IO));
        inc += (sizeof(UNUM_NATIVE_IO) / sizeof(UnumInternalNative));
        memcpy(&res->native[inc], UNUM_NATIVE_GLOBAL, sizeof(UNUM_NATIVE_GLOBAL));
    }

    // Create library string
    // Todo - Optimize by allowing for separate token sources rather than one
    size code_len = (strlen(UNUM_LIBRARY_MATH) +
                     strlen(UNUM_LIBRARY_IO) +
                     strlen(UNUM_LIBRARY_GLOBAL)
                     );
    res->code = UNUM_MALLOC(sizeof(txt) * code_len);

    memset(res->code, 0, sizeof(txt) * code_len);

    strcat(res->code, UNUM_LIBRARY_MATH);
    strcat(res->code, UNUM_LIBRARY_IO);
    strcat(res->code, UNUM_LIBRARY_GLOBAL);

    // Load standard libraries, not used (for now)
    // Todo - Load each individually to better describe errors
    // Todo - Load libraries on program initialization rather than first execution
    if(UNUM_FALSE)
    {
        unum_log("Failed to load standard libraries");

        // Return error
        UNUM_FREE(res->stack.objects);
        UNUM_FREE(res->native);
        UNUM_FREE(res->code);
        UNUM_FREE(res);
        return NULL;
    }

    return res;
}

UNUM_DEF void Unum_Destroy(UnumInstance* c)
{
    // Todo - Improve cleanup by using memory pool
    // Note - The current cleanup method may leak memory in some cases

    UnumInternalObjStack* s = &c->stack;

    // Todo - Fix
    while(s != NULL)
    {
        // Skip initial objects (primitive types)
        for(size i = (sizeof(UNUM_OBJECT_PRIMITIVES) / sizeof(UnumInternalObject)) + 1; i < s->count; i++)
        {
            // Recursively destroy objects
            // Unum_Internal_Execute_Obj_Clear(s->objects[i]);
        }

        // Set next object if it isn't in the primary stack
        if(s->count > 0 && s != &c->stack)
            s = (UnumInternalObjStack*)(s->objects[s->count - 1].data);
        else
            s = NULL;
    }

    UNUM_FREE(c->stack.objects);

    UNUM_FREE(c->program.code);
    UNUM_FREE(c->program.statement);
    UNUM_FREE(c->program.tokens);

    UNUM_FREE(c->native);

    UNUM_FREE(c->code);

    UNUM_FREE(c);

#ifdef UNUM_EXT
    p_libsys_shutdown();
#endif
}

#ifdef __cplusplus
}
#endif // C++ guard
