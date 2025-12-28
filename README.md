# 🔄 NFA Epsilon Transition Elimination

> ✨ **Efficiently remove epsilon transitions from Non-deterministic Finite Automata** ✨

---

## 📋 Team Members

| Name | Specialization |
|------|-----------------|
| **Djaballah Abdelfatah** | Security Engineering - Group 01 |
| **Derardja Mohamed** | Security Engineering - Group 01 |
| **Achouri Mohamed Tahar** | Security Engineering - Group 01 |

---

## 📖 Overview
This program automatically removes epsilon (ε) transitions from an NFA, creating an equivalent NFA without epsilon transitions while preserving the language acceptance.

---

## 🎯 What It Does

| Step | Description |
|------|-------------|
| 1️⃣ **Read NFA** | Takes input describing an NFA structure |
| 2️⃣ **Calculate Epsilon Closure** | Finds all states reachable via epsilon transitions |
| 3️⃣ **Compute New Transitions** | Creates transitions that skip epsilon moves |
| 4️⃣ **Determine Final States** | Marks states that can reach original final states |
| 5️⃣ **Display Result** | Shows the cleaned NFA with improved readability |

---

## 📥 Input Format

```
num_states
num_symbols
symbol1 symbol2 ... symbolN
initial_state
num_final_states
final_state1 final_state2 ... final_stateN
from symbol to
from symbol to
... (use 'e' for epsilon)
-1 -1 -1 (to end transitions)
```

## 📊 Example

### **Input:**
```
3
2
a b
0
1
2
0 e 1
1 a 2
2 b 0
0 b 1
-1 -1 -1
```

### **Output:**
```
Epsilon Closure:
  0 → {0, 1}
  1 → {1}
  2 → {2}

New Transitions:
  0 --a--> 2
  0 --b--> 1
  1 --a--> 2
  2 --b--> 0
  2 --b--> 1

Final States: {2}
```


---

## 🎥 Demo
![App Demo](assets/demo.gif)

---

## 📝 Notes

- ⚙️ States are numbered 0 to n-1
- ➡️ Use 'e' for epsilon transitions
- 📦 Supports up to 50 states and 26 symbols
- ✅ The result accepts the same language as the original NFA
- 🔒 Language equivalence is preserved

---

## ✅ Features

- ⚡ Fast epsilon closure computation
- 🎨 Clear formatted output
- 🔍 Easy-to-follow algorithm steps
- 💪 Robust error handling

---

<div align="center">


</div>

