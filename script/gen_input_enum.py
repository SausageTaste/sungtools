import os


works = dict()

works["mouse btn"] = [
    "left",
    "right",
    "middle",
    "eoe"
]

works["mouse action"] = [
    "move",
    "btn_up",
    "btn_down",
    "mwheel_up",
    "mwheel_down",
    "eoe"
]

works["key code"] = [
    # Alphabets
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    # Horizontal numbers
    "n0", "n1", "n2", "n3", "n4", "n5", "n6", "n7", "n8", "n9",
    # Misc in keyboard main area
    "backquote", "minus", "equal", "lbracket", "rbracket", "backslash",
    "semicolon", "quote", "comma", "period", "slash",
    # Special characters
    "space", "enter", "backspace", "tab",
    # No characters
    "escape", "lshfit", "rshfit", "lctrl", "rctrl", "lalt", "ralt", "up",
    "down", "left", "right",
    # End Of Enum, just for calculating number of elements of Enum class
    "eoe"
]


def main():
    lines = [
        "#pragma once",
        "",
    ]

    for name, items in works.items():
        name_elements = name.split()

        ENUM_NAME = "".join(x.capitalize() for x in name_elements)
        C_ELEMENT_PREFIX = "_".join(x.upper() for x in name_elements)

        lines.append(f"typedef enum Sung{ENUM_NAME} {{")
        for item in items:
            lines.append(f"    SUNG_{C_ELEMENT_PREFIX}_{item.upper()},")
        lines.append(f"}} Sung{ENUM_NAME};")
        lines.append("")

        if True:
            lines.append("#ifdef __cplusplus")
            lines.append("namespace sung {")
            lines.append("")

            lines.append(f"enum class {ENUM_NAME} {{")
            for item in items:
                lines.append(f"    {item.lower()},")
            lines.append("};")
            lines.append("")

            lines.append(f"inline {ENUM_NAME} conv_enum(const Sung{ENUM_NAME} e) {{")
            lines.append(f"    switch (e) {{")
            for item in items:
                lines.append(f"        case SUNG_{C_ELEMENT_PREFIX}_{item.upper()}:")
                lines.append(f"            return {ENUM_NAME}::{item.lower()};")
            lines.append(f"        default:")
            lines.append(f"            return {ENUM_NAME}::unknown;")
            lines.append(f"    }}")
            lines.append(f"}}")
            lines.append("")

            lines.append(f"}}")
            lines.append("#endif")
            lines.append("")

    for x in lines:
        print(x)


if __name__ == "__main__":
    main()
