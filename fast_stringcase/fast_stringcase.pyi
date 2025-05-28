def lowercase(s: str) -> str:
    """Convert the string to lowercase.

    Args:
        s (str): The input string.

    Returns:
        str: The lowercase version of the input string.

    Example:
        >>> lowercase("HeLLo")
        "hello"
    """
    ...
def uppercase(s: str) -> str:
    """Convert the string to uppercase.

    Args:
        s (str): The input string.

    Returns:
        str: The uppercase version of the input string.

    Example:
        >>> uppercase("hElLo")
        "HELLO"
    """
    ...
def snakecase(s: str) -> str:
    """Convert the string to snake_case (lowercase with underscores).

    Args:
        s (str): The input string.

    Returns:
        str: The snake_cased version of the input string.

    Example:
        >>> snakecase("helloWorld")
        "hello_world"
    """
    ...
def camelcase(s: str) -> str:
    """Convert the string to camelCase (lowercase first letter, uppercase after separators).

    Args:
        s (str): The input string.

    Returns:
        str: The camelCased version of the input string.

    Example:
        >>> camelcase("Hello_World")
        "helloWorld"
    """
    ...
def capitalcase(s: str) -> str:
    """Capitalize each word's first letter except the first (e.g., 'hello world' → 'hello World').

    Args:
        s (str): The input string.

    Returns:
        str: The capitalized string.

    Example:
        >>> capitalcase("hello world")
        "hello World"
    """
    ...
def pascalcase(s: str) -> str:
    """Convert the string to PascalCase (each word starts with uppercase, no underscores).

    Args:
        s (str): The input string.

    Returns:
        str: The PascalCased version of the input string.

    Example:
        >>> pascalcase("hello_world")
        "HelloWorld"
    """
    ...
def constcase(s: str) -> str:
    """Convert the string to UPPERCASE_WITH_UNDERSCORES (constant case).

    Args:
        s (str): The input string.

    Returns:
        str: The CONSTANT_CASED version of the input string.

    Example:
        >>> constcase("helloWorld")
        "HELLO_WORLD"
    """
    ...
def pathcase(s: str) -> str:
    """Convert the string to path/case using slashes.

    Args:
        s (str): The input string.

    Returns:
        str: The path/cased version of the input string.

    Example:
        >>> pathcase("helloWorld")
        "hello/world"
    """
    ...
def backslashcase(s: str) -> str:
    """Convert the string to path\case using backslashes.

    Args:
        s (str): The input string.

    Returns:
        str: The path\cased version of the input string.

    Example:
        >>> backslashcase("helloWorld")
        "hello\\world"
    """
    ...
def spinalcase(s: str) -> str:
    """Convert the string to spinal-case using hyphens.

    Args:
        s (str): The input string.

    Returns:
        str: The spinal-cased version of the input string.

    Example:
        >>> spinalcase("helloWorld")
        "hello-world"
    """
    ...
def dotcase(s: str) -> str:
    """Convert the string to dot.case using periods.

    Args:
        s (str): The input string.

    Returns:
        str: The dot.cased version of the input string.

    Example:
        >>> dotcase("helloWorld")
        "hello.world"
    """
    ...
def titlecase(s: str) -> str:
    """Convert the string to Title Case Each Word.

    Args:
        s (str): The input string.

    Returns:
        str: The Title Cased version of the input string.

    Example:
        >>> titlecase("hello world")
        "Hello World"
    """
    ...
def trimcase(s: str) -> str:
    """Trim whitespace from the start and end of the string.

    Args:
        s (str): The input string.

    Returns:
        str: The string with leading/trailing whitespace removed.

    Example:
        >>> trimcase("  hello  ")
        "hello"
    """
    ...
def alphanumcase(s: str) -> str:
    """Remove non-alphanumeric characters from the string.

    Args:
        s (str): The input string.

    Returns:
        str: The string with non-alphanumeric characters stripped.

    Example:
        >>> alphanumcase("hello!@#World")
        "helloWorld"
    """
    ...
def sentencecase(s: str) -> str:
    """Capitalize the first letter of the sentence and lowercase the rest.

    Args:
        s (str): The input string.

    Returns:
        str: The sentence-cased version of the input string.

    Example:
        >>> sentencecase("HELLO WORLD")
        "Hello world"
    """
    ...
