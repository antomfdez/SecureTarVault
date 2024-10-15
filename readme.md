
# SecureTarVault

**SecureTarVault** is a simple yet powerful command-line tool that enables encryption and decryption of multiple files. It creates a `tar` archive from the specified file paths, then encrypts it using GPG. You can also decrypt `.gpg` files and restore the original files. It offers flexible options to specify the output file for encryption with the `-o` flag.

## Features

- **Encrypt Multiple Files**: Select multiple files to compress and encrypt into a single `.gpg` archive.
- **Decrypt Encrypted Files**: Decrypt a `.gpg` file and restore its original contents.
- **Flexible Output**: Specify the output filename for the encrypted `.gpg` file using the `-o` flag at any position in the command line.
- **Automatic Cleanup**: Automatically removes temporary files after encryption or decryption to ensure a tidy process.

## Usage

### Encrypt Files

To encrypt one or more files using a public key:

```bash
./securetarvault c <public_key_id> -o <output_file> <file_path1> [file_path2] ...
```

You can specify the -o <output_file> at any position. For example:

```bash
./securetarvault c <public_key_id> file1.txt -o encrypted_output.gpg file2.txt
./securetarvault c -o encrypted_output.gpg <public_key> file1.txt file2.txt
```

This will create a tar archive containing the specified files and encrypt it using the provided public key. The encrypted output will be saved as the specified <output_file> (e.g., encrypted_output.gpg).
Decrypt Files

To decrypt an encrypted .gpg file and extract the contents:

```bash
./securetarvault d <encrypted_file.gpg>
```

This will decrypt the provided .gpg file, extract the contents from the embedded .tar archive, and restore the original files.
Cleanup

  -  After Encryption: The intermediate files.tar file is automatically deleted after encryption is complete.
  -  After Decryption: The files.tar file is automatically removed after extraction of the original files.

## Build

To compile the project, use gcc:

```bash
gcc -o securetarvault securetarvault.c
```

## Example

Encrypt files using a public key:

```bash
./securetarvault c my_public_key_id -o encrypted_data.gpg file1.txt file2.txt
```

### Decrypt a file:

```bash
./securetarvault d encrypted_data.gpg
```

### Requirements

    GPG: Ensure GPG is installed on your system.
    tar: The tar utility is used to archive files before encryption.

# License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).