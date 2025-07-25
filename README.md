# Duplicate Detector (Nozomi Challenge)

This project implements a memory-efficient and fast duplicate character detector written in C.

It was originally submitted as a technical challenge to **Nozomi Networks** in early 2025.  
Publishing it here — for visibility, for learning, and for fun.

---

## 🧠 The Challenge

> Write an algorithm (in any language) that, given a character string like `{‘c’,’a’,’i’,’o’,’p’,’a’}`,  
> prints the list of characters appearing at least twice.  
> Afterwards, comment on the space and time complexity.

---

## ✨ My Approach

- **Language**: C (with low-level optimizations in mind)
- **Memory Usage**: Fixed-size `uint8_t[64]` bitfield for tracking characters
- **Scope**: Only visible ASCII characters (32 to 126)
- **No dynamic memory**, no libraries, no malloc
- **Output**: Printed to stdout in a single buffered write operation
- **Design Considerations**: 
  - Targeted for performance and I/O efficiency
  - Annotated for portability across platforms
  - Includes comments about tradeoffs depending on CPU architecture

---

## 🚀 Time & Space Complexity

- **Time**: O(n), where `n` is the length of the input string
- **Space**: O(1), constant memory usage (512 bits + small result buffer)

---

## ⚙️ Build & Run

### Compile with GCC

```bash
gcc -O3 -o dupdetector main.c find_duplicates.c
```

## 📜 License

[MIT License](LICENSE)

You’re free to use, modify, or study this code.  

---


##✍️ Author

Moulay Abdellah Idrissi
Software Architect | Embedded Systems | DevOps | Full Stack
🔗 linkedin.com/in/maidrissi
📧 a.idrissi.dev@gmail.com
