# 📌 Data Compression & Decompression Using Huffman and RLE  
A high-performance **data compression and decompression system** utilizing **Huffman Coding and Run-Length Encoding (RLE)**. This project efficiently compresses text data and provides a method for decompression using trie-based retrieval.


## 🚀 Project Overview  
This project implements a **hybrid compression technique** combining **Huffman Coding** and **Run-Length Encoding (RLE)** to reduce the size of text-based data. The system also performs **performance analysis** based on time and space complexity.

### 🔹 **Key Features**
- **Huffman Encoding** – Constructs a Huffman tree to create an optimal prefix code for compression.
- **Run-Length Encoding (RLE)** – Further compresses the Huffman-encoded data by replacing repeated sequences.
- **Trie-Based Decompression** – A prefix trie efficiently maps compressed sequences back to their original form.
- **Performance Analysis** – Theoretical and practical evaluation of time and space complexity.
- **Memory Efficiency** – Efficient use of **dynamic memory allocation** to store compressed data.


## 🧑‍🤝‍🧑 Collaboration

Project contributor:

- [Zihad Khan](https://github.com/Zihad107)

> _Thanks for the valuable contribution to the development of this system._


## 📜 **Problem Approach**
The primary goal of this project is **efficient data compression and retrieval**, minimizing memory usage while maintaining fast encoding and decoding. The approach consists of:
1. **Input Processing** – The system reads input text and calculates character frequency.
2. **Huffman Tree Construction** – The tree is built using a **Min Heap**, assigning shorter binary codes to more frequent characters.
3. **Huffman Encoding** – Text is encoded into a binary sequence using the generated codes.
4. **RLE Compression** – The Huffman-encoded sequence undergoes **Run-Length Encoding** to remove redundancy.
5. **Storage in Trie Structure** – The encoded sequence is stored in a Trie for efficient decompression.
6. **Decoding & Decompression** – RLE-decoded data is mapped back using the Huffman tree to retrieve the original text.



## 🏛 **System Architecture**
| **Component** | **Description** |
|--------------|----------------|
| **Huffman Encoding** | Generates optimal prefix-free binary codes for characters. |
| **Min Heap Structure** | Organizes nodes for Huffman tree construction. |
| **Run-Length Encoding (RLE)** | Compresses repetitive Huffman codes. |
| **Trie-Based Decoding** | Efficiently stores and retrieves Huffman codes for decompression. |

### **Time Complexity Analysis**
- **Huffman Encoding**: **O(n log₂ n)** (Using Min Heap)
- **Run-Length Encoding**: **O(k)**
- **Decompression**: **O(n + k)**
- **Overall Complexity**: **O(n log₂ n)** (Dominant Factor)

### **Space Complexity Analysis**
- Huffman Tree: **O(n)**
- Trie Structure: **O(k)**
- Encoded Text Storage: **O(n)**
- **Overall Space Complexity**: **O(n)**



## 🛠 **Installation & Setup**
### **1️⃣ Clone the Repository**
```bash
git clone https://github.com/your-username/data-compression.git
cd data-compression
```

### **2️⃣ Compile the Program**
Use GCC to compile the C program:
```bash
gcc -o compression UPDATED.c
```

### **3️⃣ Run the Program**
```bash
./compression
```
It will prompt the user to enter the length of text for compression.



## 📊 **Performance Analysis**
Theoretical and practical performance comparisons were conducted using datasets ranging from **1,000 to 1,000,000** characters.  

### **📈 Time Complexity Graph**
- **Theoretical:** **O(n log₂ n)**
- **Practical:** Shows increasing execution time for larger datasets due to computational overhead.

### **📉 Space Complexity Graph**
- **Theoretical:** **O(n)**
- **Practical:** Uses **~10% of the theoretical space** in real-world scenarios, demonstrating memory efficiency.



## 🏆 **Advantages**
✅ **Efficient Encoding & Decoding**  
✅ **Reduced Memory Usage**  
✅ **Faster Decompression with Trie Structure**  
✅ **Optimized Huffman Tree Implementation**  
✅ **Handles Large Datasets Efficiently**  
