//
// Created by SKIKK on 07/11/2023.
//

#ifndef BRACKOCALYPSE_KEYMAP_HPP
#define BRACKOCALYPSE_KEYMAP_HPP

#define BRACK_KEY_MAP(X)

enum KeyMap {
    UNKNOWN = 0,
    RETURN = '\r',
    ESCAPE = '\x1B',
    BACKSPACE = '\b',
    TAB = '\t',
    SPACE = ' ',
    EXCLAIM = '!',
    QUOTEDBL = '"',
    HASH = '#',
    PERCENT = '%',
    DOLLAR = '$',
    AMPERSAND = '&',
    QUOTE = '\'',
    LEFTPAREN = '(',
    RIGHTPAREN = ')',
    ASTERISK = '*',
    PLUS = '+',
    COMMA = ',',
    MINUS = '-',
    PERIOD = '.',
    SLASH = '/',
    _0 = '0',
    _1 = '1',
    _2 = '2',
    _3 = '3',
    _4 = '4',
    _5 = '5',
    _6 = '6',
    _7 = '7',
    _8 = '8',
    _9 = '9',
    COLON = ':',
    SEMICOLON = ';',
    LESS = '<',
    EQUALS = '=',
    GREATER = '>',
    QUESTION = '?',
    AT = '@',
    LEFTBRACKET = '[',
    BACKSLASH = '\\',
    RIGHTBRACKET = ']',
    CARET = '^',
    UNDERSCORE = '_',
    BACKQUOTE = '`',
    a = 'a',
    b = 'b',
    c = 'c',
    d = 'd',
    e = 'e',
    f = 'f',
    g = 'g',
    h = 'h',
    i = 'i',
    j = 'j',
    k = 'k',
    l = 'l',
    m = 'm',
    n = 'n',
    o = 'o',
    p = 'p',
    q = 'q',
    r = 'r',
    s = 's',
    t = 't',
    u = 'u',
    v = 'v',
    w = 'w',
    x = 'x',
    y = 'y',
    z = 'z',
    CAPSLOCK = 1073741881,
    F1 = 1073741882,
    F2 = 1073741883,
    F3 = 1073741884,
    F4 = 1073741885,
    F5 = 1073741886,
    F6 = 1073741887,
    F7 = 1073741888,
    F8 = 1073741889,
    F9 = 1073741890,
    F10 = 1073741891,
    F11 = 1073741892,
    F12 = 1073741893,
    PRINTSCREEN = 1073741894,
    SCROLLLOCK = 1073741895,
    PAUSE = 1073741896,
    INSERT = 1073741897,
    HOME = 1073741898,
    PAGEUP = 1073741899,
    DELETE = 127,
    END = 1073741901,
    PAGEDOWN = 1073741902,
    RIGHT = 1073741903,
    LEFT = 1073741904,
    DOWN = 1073741905,
    UP = 1073741906,
    NUMLOCKCLEAR = 1073741907,
    KP_DIVIDE = 1073741908,
    KP_MULTIPLY = 1073741909,
    KP_MINUS = 1073741910,
    KP_PLUS = 1073741911,
    KP_ENTER = 1073741912,
    KP_1 = 1073741913,
    KP_2 = 1073741914,
    KP_3 = 1073741915,
    KP_4 = 1073741916,
    KP_5 = 1073741917,
    KP_6 = 1073741918,
    KP_7 = 1073741919,
    KP_8 = 1073741920,
    KP_9 = 1073741921,
    KP_0 = 1073741922,
    KP_PERIOD = 1073741923,
    APPLICATION = 1073741925,
    POWER = 1073741926,
    KP_EQUALS = 1073741927,
    F13 = 1073741928,
    F14 = 1073741929,
    F15 = 1073741930,
    F16 = 1073741931,
    F17 = 1073741932,
    F18 = 1073741933,
    F19 = 1073741934,
    F20 = 1073741935,
    F21 = 1073741936,
    F22 = 1073741937,
    F23 = 1073741938,
    F24 = 1073741939,
    EXECUTE = 1073741940,
    HELP = 1073741941,
    MENU = 1073741942,
    SELECT = 1073741943,
    STOP = 1073741944,
    AGAIN = 1073741945,
    UNDO = 1073741946,
    CUT = 1073741947,
    COPY = 1073741948,
    PASTE = 1073741949,
    FIND = 1073741950,
    MUTE = 1073741951,
    VOLUMEUP = 1073741952,
    VOLUMEDOWN = 1073741953,
    KP_COMMA = 1073741957,
    KP_EQUALSAS400 = 1073741958,
    ALTERASE = 1073741977,
    SYSREQ = 1073741978,
    CANCEL = 1073741979,
    CLEAR = 1073741980,
    PRIOR = 1073741981,
    RETURN2 = 1073741982,
    SEPARATOR = 1073741983,
    OUT = 1073741984,
    OPER = 1073741985,
    CLEARAGAIN = 1073741986,
    CRSEL = 1073741987,
    EXSEL = 1073741988,
    KP_00 = 1073742000,
    KP_000 = 1073742001,
    THOUSANDSSEPARATOR = 1073742002,
    DECIMALSEPARATOR = 1073742003,
    CURRENCYUNIT = 1073742004,
    CURRENCYSUBUNIT = 1073742005,
    KP_LEFTPAREN = 1073742006,
    KP_RIGHTPAREN = 1073742007,
    KP_LEFTBRACE = 1073742008,
    KP_RIGHTBRACE = 1073742009,
    KP_TAB = 1073742010,
    KP_BACKSPACE = 1073742011,
    KP_A = 1073742012,
    KP_B = 1073742013,
    KP_C = 1073742014,
    KP_D = 1073742015,
    KP_E = 1073742016,
    KP_F = 1073742017,
    KP_XOR = 1073742018,
    KP_POWER = 1073742019,
    KP_PERCENT = 1073742020,
    KP_LESS = 1073742021,
    KP_GREATER = 1073742022,
    KP_AMPERSAND = 1073742023,
    KP_DBLAMPERSAND = 1073742024,
    KP_VERTICALBAR = 1073742025,
    KP_DBLVERTICALBAR = 1073742026,
    KP_COLON = 1073742027,
    KP_HASH = 1073742028,
    KP_SPACE = 1073742029,
    KP_AT = 1073742030,
    KP_EXCLAM = 1073742031,
    KP_MEMSTORE = 1073742032,
    KP_MEMRECALL = 1073742033,
    KP_MEMCLEAR = 1073742034,
    KP_MEMADD = 1073742035,
    KP_MEMSUBTRACT = 1073742036,
    KP_MEMMULTIPLY = 1073742037,
    KP_MEMDIVIDE = 1073742038,
    KP_PLUSMINUS = 1073742039,
    KP_CLEAR = 1073742040,
    KP_CLEARENTRY = 1073742041,
    KP_BINARY = 1073742042,
    KP_OCTAL = 1073742043,
    KP_DECIMAL = 1073742044,
    KP_HEXADECIMAL = 1073742045,
    LCTRL = 1073742048,
    LSHIFT = 1073742049,
    LALT = 1073742050,
    LGUI = 1073742051,
    RCTRL = 1073742052,
    RSHIFT = 1073742053,
    RALT = 1073742054,
    RGUI = 1073742055,
    MODE = 1073742081,
    AUDIONEXT = 1073742082,
    AUDIOPREV = 1073742083,
    AUDIOSTOP = 1073742084,
    AUDIOPLAY = 1073742085,
    AUDIOMUTE = 1073742086,
    MEDIASELECT = 1073742087,
    WWW = 1073742088,
    MAIL = 1073742089,
    CALCULATOR = 1073742090,
    COMPUTER = 1073742091,
    AC_SEARCH = 1073742092,
    AC_HOME = 1073742093,
    AC_BACK = 1073742094,
    AC_FORWARD = 1073742095,
    AC_STOP = 1073742096,
    AC_REFRESH = 1073742097,
    AC_BOOKMARKS = 1073742098,
    BRIGHTNESSDOWN = 1073742099,
    BRIGHTNESSUP = 1073742100,
    DISPLAYSWITCH = 1073742101,
    KBDILLUMTOGGLE = 1073742102,
    KBDILLUMDOWN = 1073742103,
    KBDILLUMUP = 1073742104,
    EJECT = 1073742105,
    SLEEP = 1073742106,
    APP1 = 1073742107,
    APP2 = 1073742108,
    AUDIOREWIND = 1073742109,
    AUDIOFASTFORWARD = 1073742110,
    SOFTLEFT = 1073742111,
    SOFTRIGHT = 1073742112,
    CALL = 1073742113,
    ENDCALL = 1073742114
};

#define KeyMap_List \
    UNKNOWN, \
    RETURN, \
    ESCAPE, \
    BACKSPACE, \
    TAB, \
    SPACE, \
    EXCLAIM, \
    QUOTEDBL, \
    HASH, \
    PERCENT, \
    DOLLAR, \
    AMPERSAND, \
    QUOTE, \
    LEFTPAREN, \
    RIGHTPAREN, \
    ASTERISK, \
    PLUS, \
    COMMA, \
    MINUS, \
    PERIOD, \
    SLASH, \
    _0, \
    _1, \
    _2, \
    _3, \
    _4, \
    _5, \
    _6, \
    _7, \
    _8, \
    _9, \
    COLON, \
    SEMICOLON, \
    LESS, \
    EQUALS, \
    GREATER, \
    QUESTION, \
    AT, \
    LEFTBRACKET, \
    BACKSLASH, \
    RIGHTBRACKET, \
    CARET, \
    UNDERSCORE, \
    BACKQUOTE, \
    a, \
    b, \
    c, \
    d, \
    e, \
    f, \
    g, \
    h, \
    i, \
    j, \
    k, \
    l, \
    m, \
    n, \
    o, \
    p, \
    q, \
    r, \
    s, \
    t, \
    u, \
    v, \
    w, \
    x, \
    y, \
    z, \
    CAPSLOCK, \
    F1, \
    F2, \
    F3, \
    F4, \
    F5, \
    F6, \
    F7, \
    F8, \
    F9, \
    F10, \
    F11, \
    F12, \
    PRINTSCREEN, \
    SCROLLLOCK, \
    PAUSE, \
    INSERT, \
    HOME, \
    PAGEUP, \
    DELETE, \
    END, \
    PAGEDOWN, \
    RIGHT, \
    LEFT, \
    DOWN, \
    UP, \
    NUMLOCKCLEAR, \
    KP_DIVIDE, \
    KP_MULTIPLY, \
    KP_MINUS, \
    KP_PLUS, \
    KP_ENTER, \
    KP_1, \
    KP_2, \
    KP_3, \
    KP_4, \
    KP_5, \
    KP_6, \
    KP_7, \
    KP_8, \
    KP_9, \
    KP_0, \
    KP_PERIOD, \
    APPLICATION, \
    POWER, \
    KP_EQUALS, \
    F13, \
    F14, \
    F15, \
    F16, \
    F17, \
    F18, \
    F19, \
    F20, \
    F21, \
    F22, \
    F23, \
    F24, \
    EXECUTE, \
    HELP, \
    MENU, \
    SELECT, \
    STOP, \
    AGAIN, \
    UNDO, \
    CUT, \
    COPY, \
    PASTE, \
    FIND, \
    MUTE, \
    VOLUMEUP, \
    VOLUMEDOWN, \
    KP_COMMA, \
    KP_EQUALSAS400, \
    ALTERASE, \
    SYSREQ, \
    CANCEL, \
    CLEAR, \
    PRIOR, \
    RETURN2, \
    SEPARATOR, \
    OUT, \
    OPER, \
    CLEARAGAIN, \
    CRSEL, \
    EXSEL, \
    KP_00, \
    KP_000, \
    THOUSANDSSEPARATOR, \
    DECIMALSEPARATOR, \
    CURRENCYUNIT, \
    CURRENCYSUBUNIT, \
    KP_LEFTPAREN, \
    KP_RIGHTPAREN, \
    KP_LEFTBRACE, \
    KP_RIGHTBRACE, \
    KP_TAB, \
    KP_BACKSPACE, \
    KP_A, \
    KP_B, \
    KP_C, \
    KP_D, \
    KP_E, \
    KP_F, \
    KP_XOR, \
    KP_POWER, \
    KP_PERCENT, \
    KP_LESS, \
    KP_GREATER, \
    KP_AMPERSAND, \
    KP_DBLAMPERSAND, \
    KP_VERTICALBAR, \
    KP_DBLVERTICALBAR, \
    KP_COLON, \
    KP_HASH, \
    KP_SPACE, \
    KP_AT, \
    KP_EXCLAM, \
    KP_MEMSTORE, \
    KP_MEMRECALL, \
    KP_MEMCLEAR, \
    KP_MEMADD, \
    KP_MEMSUBTRACT, \
    KP_MEMMULTIPLY, \
    KP_MEMDIVIDE, \
    KP_PLUSMINUS, \
    KP_CLEAR, \
    KP_CLEARENTRY, \
    KP_BINARY, \
    KP_OCTAL, \
    KP_DECIMAL, \
    KP_HEXADECIMAL, \
    LCTRL, \
    LSHIFT, \
    LALT, \
    LGUI, \
    RCTRL, \
    RSHIFT, \
    RALT, \
    RGUI, \
    MODE, \
    AUDIONEXT, \
    AUDIOPREV, \
    AUDIOSTOP, \
    AUDIOPLAY, \
    AUDIOMUTE, \
    MEDIASELECT, \
    WWW, \
    MAIL, \
    CALCULATOR, \
    COMPUTER, \
    AC_SEARCH, \
    AC_HOME, \
    AC_BACK, \
    AC_FORWARD, \
    AC_STOP, \
    AC_REFRESH, \
    AC_BOOKMARKS, \
    BRIGHTNESSDOWN, \
    BRIGHTNESSUP, \
    DISPLAYSWITCH, \
    KBDILLUMTOGGLE, \
    KBDILLUMDOWN, \
    KBDILLUMUP, \
    EJECT, \
    SLEEP, \
    APP1, \
    APP2, \
    AUDIOREWIND, \
    AUDIOFASTFORWARD, \
    SOFTLEFT, \
    SOFTRIGHT, \
    CALL, \
    ENDCALL

#endif //BRACKOCALYPSE_KEYMAP_HPP