# Password Generator
- [x] Password Generator Implementation
# Encryption 
- [x] PBKDF2\<SHA256> derives encryption key from password
- [x] Generate random nonce  
- [ ] Read plaintext file to memory  
- [ ] Encrypt with AES-256-GCM (includes authentication tag)  
- [ ] Write header + nonce + ciphertext + tag to disk  

# Decryption
- [x] PBKDF2\<SHA256> derives same encryption key from same password and same salt
- [ ] Read encrypted file header to get parameters  
- [ ] Verify authentication tag  
- [ ] Decrypt to memory buffer  
- [ ] User edits in-memory content  
- [ ] Generate new nonce  
- [ ] Re-encrypt modified content  
- [ ] Write to temp file, then atomically replace original  
- [ ] Zeroize all memory buffers  