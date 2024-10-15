#!/bin/bash

# Define the installation path to the user's hidden ~/.local/bin directory
INSTALL_PATH="$HOME/.local/bin"

# Check if ~/.local/bin exists, if not, create it
if [ ! -d "$INSTALL_PATH" ]; then
    mkdir -p "$INSTALL_PATH"
    echo "Created $INSTALL_PATH directory."
fi

# Compile the program
gcc -o stv securetarvault.c

# Check if the compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    
    # Move the binary to ~/.local/bin
    mv stv "$INSTALL_PATH/"
    
    if [ $? -eq 0 ]; then
        echo "SecureTarVault installed successfully in $INSTALL_PATH."
        echo "You can now use 'stv' from the terminal."
    else
        echo "Failed to move the binary to $INSTALL_PATH. Please try manually."
    fi
else
    echo "Compilation failed. Please check for errors."
fi

# Check if ~/.local/bin is in the user's PATH
if [[ ":$PATH:" != *":$INSTALL_PATH:"* ]]; then
    echo "Adding $INSTALL_PATH to your PATH."

    # Determine the user's shell and update the appropriate configuration file
    SHELL_NAME=$(basename "$SHELL")

    case "$SHELL_NAME" in
        bash)
            echo "export PATH=\"\$HOME/.local/bin:\$PATH\"" >> "$HOME/.bashrc"
            echo "Please run 'source ~/.bashrc' to update your current shell."
            ;;
        zsh)
            echo "export PATH=\"\$HOME/.local/bin:\$PATH\"" >> "$HOME/.zshrc"
            echo "Please run 'source ~/.zshrc' to update your current shell."
            ;;
        fish)
            echo "set -U fish_user_paths \$HOME/.local/bin \$fish_user_paths" >> "$HOME/.config/fish/config.fish"
            echo "Please restart your terminal or run 'source ~/.config/fish/config.fish' to update your current shell."
            ;;
        *)
            echo "Unknown shell: $SHELL_NAME"
            echo "Please add '$INSTALL_PATH' to your PATH manually."
            ;;
    esac
fi
