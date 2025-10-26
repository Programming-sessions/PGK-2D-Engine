import os

def gather_code_files(start_dir):
    """
    Gathers content from .cpp and .h files in a directory and its subdirectories.
    Automatically detects file encoding.

    Args:
        start_dir (str): The directory to start the search from.

    Returns:
        dict: A dictionary where keys are file paths and values are the content of the files.
    """
    code_files = {}
    for root, _, files in os.walk(start_dir):
        # Only process files in the current directory or subdirectories of 'src'
        if root == start_dir or 'src' in os.path.relpath(root, start_dir).split(os.sep):
            for file in files:
                if file.endswith(('.cpp', '.h')):
                    file_path = os.path.join(root, file)
                    try:
                        # Try UTF-16 LE first (common Visual Studio encoding)
                        try:
                            with open(file_path, 'r', encoding='utf-16-le') as f:
                                code_files[file_path] = f.read()
                        except (UnicodeDecodeError, UnicodeError):
                            # If UTF-16 fails, try UTF-8
                            try:
                                with open(file_path, 'r', encoding='utf-8') as f:
                                    code_files[file_path] = f.read()
                            except (UnicodeDecodeError, UnicodeError):
                                # Finally fall back to latin-1 (never fails)
                                with open(file_path, 'r', encoding='latin-1') as f:
                                    code_files[file_path] = f.read()
                    except Exception as e:
                        print(f"Error reading file: {file_path} - {e}")
    return code_files

def create_markdown_file(code_data, output_path):
    """
    Creates a markdown file from the gathered code data using UTF-8 encoding with proper code blocks.

    Args:
        code_data (dict): A dictionary where keys are file paths and values are the content of the files.
        output_path (str): The path to the output markdown file.
    """
    try:
        with open(output_path, 'w', encoding='utf-8') as md_file:
            for file_path, content in code_data.items():
                md_file.write(f"## {file_path}\n\n")
                # Determine language based on file extension
                lang = 'cpp' if file_path.endswith('.cpp') or file_path.endswith('.h') else ''
                # Use string concatenation instead of f-string for backticks
                md_file.write("```" + lang + "\n")
                md_file.write(content)
                md_file.write("\n```\n\n")
        print(f"Code content written to: {output_path}")
    except Exception as e:
        print(f"Error writing markdown file: {e}")

if __name__ == "__main__":
    current_dir = os.getcwd()
    
    # Gather files from the main directory and the 'src' subdirectory
    all_code = gather_code_files(current_dir)

    if all_code:
        create_markdown_file(all_code, "code.md")
    else:
        print("No .cpp or .h files found in the main directory or the 'src' subdirectory.")
