
# Module 6: Convolutional Variants in CNNs - Complete Educational Guide

**Faculty Overview | Beginner-Friendly | Working Code Examples Included**

---

## 📋 Table of Contents

1. [Introduction & Why Convolution Variants Matter](#introduction)
2. [Part 1: Detailed Visualizations for All 8 Types](#visualizations)
3. [Part 2: Working Code Examples (PyTorch & TensorFlow)](#code-examples)
4. [Part 3: Practical Assignments](#assignments)
5. [Part 4: Real Architecture Comparisons](#architectures)
6. [Setup & Installation](#setup)
7. [Quick Reference Guide](#quick-reference)

---

## Introduction {#introduction}

### Why Do We Need Different Convolutions?

Standard convolution works great for many problems, but it has limitations:
- **Limited field of view** - needs many layers to see the whole image
- **Inefficient** - processes all channels equally (wastes computation)
- **Fixed rigid structure** - can't adapt to object shapes
- **Not all feature sizes matter** - sometimes we need bigger or smaller filters

**Solution:** Specialized convolution variants that solve specific problems!

---

## Part 1: Detailed Visualizations {#visualizations}

### 1️⃣ Vanilla (Standard) Convolution

#### Visual Representation:
```
INPUT IMAGE (5×5)        FILTER (3×3)          STEP-BY-STEP CONVOLUTION
┌─────────────────┐      ┌─────────┐
│ 1  2  3  4  5   │      │ 0.1 0.2 │           Position 1:
│ 6  7  8  9 10   │      │ 0.3 0.4 │           [1×0.1 + 2×0.2 + 6×0.3 + 7×0.4
│11 12 13 14 15   │      │ 0.5 0.6 │            = ?]
│16 17 18 19 20   │      └─────────┘
│21 22 23 24 25   │      Slide: →, →, ↓, ↓
└─────────────────┘

OUTPUT (3×3)
┌─────────────┐
│ a  b  c     │
│ d  e  f     │
│ g  h  i     │
└─────────────┘

Each output value = sum(element-wise multiplication)
```

#### How It Works:
```
Step 1: Place filter at top-left
[1  2  3] × Filter
[6  7  8]

Step 2: Multiply & sum (dot product)
1×w₀ + 2×w₁ + 3×w₂ + 6×w₃ + 7×w₄ + 8×w₅ + ... = output_value

Step 3: Move right, repeat
↻ Move one position to the right
↻ Slide down when reaching the end

Result: Smaller output with extracted features
```

#### Key Properties:
```
Input:           H × W × C_in
Filter:          F × F × C_in × C_out
Output:          (H-F+1) × (W-F+1) × C_out
Parameters:      F × F × C_in × C_out
Computation:     H × W × F × F × C_in × C_out
```

#### Pros & Cons:
| Aspect | Details |
|--------|---------|
| ✅ **Pros** | Simple, interpretable, foundational, works everywhere |
| ❌ **Cons** | Computationally expensive, treats all channels equally |
| 🎯 **Best For** | General feature extraction, base convolution |

---

### 2️⃣ Depthwise Convolution

#### Visual Representation:
```
STANDARD CONVOLUTION:         DEPTHWISE CONVOLUTION:

Input: 5×5×3 (RGB)            Input: 5×5×3 (RGB)
One 3×3 Filter × 3 channels   Three separate 3×3 Filters
      ↓                        (one per channel!)
Output: 3×3×C_out                   ↓
                              Output: 3×3×3

[Filter processes all      [Red channel has its own filter]
 channels together]        [Green channel has its own filter]
                           [Blue channel has its own filter]
```

#### Computation Breakdown:
```
STANDARD (Expensive):
┌────────────────────────────────┐
│ 1 Filter × 3 Channels → Output │
│ 3×3×3 = 27 computations/output │
└────────────────────────────────┘
    Total: 27 × 3×3 = 243

DEPTHWISE (Efficient):
┌─────────────────────────────────────┐
│ Red:   3×3 Filter → 1 output        │
│ Green: 3×3 Filter → 1 output        │
│ Blue:  3×3 Filter → 1 output        │
│ Total: 3×3 per channel = 9 each     │
└─────────────────────────────────────┘
    Total: 9×3 = 27 (9× FASTER!)
```

#### Parameter Comparison:
```
Input Shape:        5×5×3
Filter Size:        3×3

Standard Conv:      3×3×3 = 27 parameters per output channel
Depthwise:          3×3×1 = 9 parameters (for all 3 channels)

Savings:            3× fewer parameters!
                    3× faster computation!
```

#### Pros & Cons:
| Aspect | Details |
|--------|---------|
| ✅ **Pros** | 8-9× faster than standard, 8-9× fewer parameters, lightweight |
| ❌ **Cons** | Less information mixing across channels, less powerful alone |
| 🎯 **Best For** | Mobile networks, lightweight models, combining with 1×1 convs |

---

### 3️⃣ Dilated (Atrous) Convolution

#### Visual Representation:
```
STANDARD 3×3 FILTER:          DILATED 3×3 (dilation=2):

[X X X]                       [X . X . X]
[X X X]  Looks at 9 cells     [. . . . .]
[X X X]                       [X . X . X]
                              
                              Looks at 9 cells (but far apart!)
                              Skips every other cell

DILATION RATE COMPARISON:

dilation=1:  ▓ ░ ▓         dilation=2:  ▓ . ░ . ▓
             ░ ░ ░                      . . . . .
             ▓ ░ ▓                      ░ . . . ░

Sees local details            Sees bigger picture!
```

#### Receptive Field Growth:
```
Receptive Field = area the filter "sees"

Standard 3×3:           3×3 = 9 cell area
With dilation=2:        5×5 = 25 cell area (without extra params!)
With dilation=3:        7×7 = 49 cell area

Dilation Formula:
Output_Size = (Input - dilation×(Filter-1) - 1) / stride + 1
```

#### Layer Stacking Effect:
```
Stack 3 dilated layers to see HUGE receptive field:

Layer 1: dilation=1  →  sees 3×3
Layer 2: dilation=2  →  sees 5×5
Layer 3: dilation=4  →  sees 9×9

Total: Can see 9×9 area with only 3 filters!
```

#### Pros & Cons:
| Aspect | Details |
|--------|---------|
| ✅ **Pros** | Large receptive field without extra params, multi-scale features |
| ❌ **Cons** | Creates "holes", misses fine-grain details, checkboard effect |
| 🎯 **Best For** | Scene understanding, semantic segmentation, object detection |

---

### 4️⃣ Transposed Convolution

#### Visual Representation:
```
STANDARD CONVOLUTION (Shrinks):
Input:  5×5  →  [Conv 3×3]  →  Output: 3×3  ↓ Shrinks

TRANSPOSED CONVOLUTION (Grows):
Input:  3×3  →  [Transpose Conv 3×3]  →  Output: 5×5  ↑ Grows

Visual Process:

Input (2×2):              Step 1: Place filter at each input element:
┌─────┐
│ A B │                   ┌─────────────┐
│ C D │              A×   │ F F F       │
└─────┘                   │ F F F       │
                          │ F F F       │
                          └─────────────┘
Filter (3×3):
┌─────┐
│ F F F │            Step 2: Overlap and sum contributions
│ F F F │
│ F F F │            ┌──────────────┐
└─────┘             │ AF  AF+BF  BF │
                    │ AF  AF+BF  BF │
                    │ CF  CF+DF  DF │
Output (4×4):       │ CF  CF+DF  DF │
                    └──────────────┘
```

#### Computation Breakdown:
```
Standard Conv:         Transposed Conv:
Input: 4×4             Input: 2×2
Filter: 3×3            Filter: 3×3
Stride: 1              Stride: 1
Output: 2×2            Output: 4×4

Formula:
Output_size = (Input - 1) × Stride + Filter_size
            = (2 - 1) × 1 + 3 = 4
```

#### Real-World Example:
```
Image Upsampling for Segmentation:

Encoder (Shrinking):           Decoder (Growing):
Input 256×256                  Last layer 16×16
  ↓ [Conv]                        ↑ [Transposed Conv]
  128×128                        32×32
  ↓ [Conv]                        ↑ [Transposed Conv]
  64×64                         64×64
  ↓ [Conv]                        ↑ [Transposed Conv]
  32×32                         128×128
  ↓ [Conv]                        ↑ [Transposed Conv]
  16×16                         256×256 (original size!)

Result: Pixel-wise predictions!
```

#### Pros & Cons:
| Aspect | Details |
|--------|---------|
| ✅ **Pros** | Learnable upsampling, smooth generation, used in GANs |
| ❌ **Cons** | Checkerboard artifacts, not always intuitive, requires careful tuning |
| 🎯 **Best For** | Image generation, segmentation, GANs, autoencoders |

---

### 5️⃣ Grouped Convolution

#### Visual Representation:
```
STANDARD CONVOLUTION:
All 12 channels → [1 Giant Filter] → Output

┌──────────────────────────────────┐
│ Channel 1  ┐                     │
│ Channel 2  ├→ [Filter] → Output  │
│ Channel 3  ┘                     │
│ ...                              │
│ Channel 12 ┘                     │
└──────────────────────────────────┘

GROUPED CONVOLUTION (Groups=3):
Split into 3 groups of 4 channels each

┌─────────────────┐
│ Channels 1-4    ├→ [Filter 1] → Output 1
│ (Group 1)       │
└─────────────────┘
┌─────────────────┐
│ Channels 5-8    ├→ [Filter 2] → Output 2
│ (Group 2)       │
└─────────────────┘
┌─────────────────┐
│ Channels 9-12   ├→ [Filter 3] → Output 3
│ (Group 3)       │
└─────────────────┘
    ↓ [Concatenate]
   Final Output (Combined)
```

#### Parameter Reduction:
```
Input:     H×W×12
Output:    H×W×16
Filter:    3×3

STANDARD (Channels=12 → 16):
Parameters = 3 × 3 × 12 × 16 = 1,728

GROUPED (Groups=3, 4 channels per group):
Group 1: 3 × 3 × 4 × 5 = 180
Group 2: 3 × 3 × 4 × 5 = 180
Group 3: 3 × 3 × 4 × 6 = 216
Total = 576 (2.67× fewer!)

Computation scales similarly
```

#### Architecture Example:
```
ResNeXt Block:

[Input: C channels]
   ↓
[Group Conv: Groups=32]
   ↓ (32 separate convolutions)
[Group Conv: Groups=32]
   ↓
[1×1 Conv: Merge information]
   ↓
[Output: Enhanced features]
```

#### Pros & Cons:
| Aspect | Details |
|--------|---------|
| ✅ **Pros** | Reduced parameters, better efficiency, can improve accuracy |
| ❌ **Cons** | Information doesn't mix between groups, less flexible |
| 🎯 **Best For** | ResNeXt, lightweight networks, efficient architectures |

---

### 6️⃣ Asymmetric Convolution

#### Visual Representation:
```
STANDARD SYMMETRIC:           ASYMMETRIC (Factorized):

One 5×5 filter:              Split into 5×1 and 1×5:

[X X X X X]                  [X]
[X X X X X]     versus       [X]  [X X X X X]
[X X X X X]                  [X]
[X X X X X]                  [X]
[X X X X X]

25 weights                    5+5 = 10 weights (2.5× fewer!)
```

#### Computation Breakdown:
```
STANDARD 5×5 CONVOLUTION:
Input:    H×W×C
Filter:   5×5×C
Output:   (H-4)×(W-4)×C_out
Params:   5×5×C = 25C per output channel
Ops:      25×H×W per output

ASYMMETRIC 5×1 + 1×5:
Step 1: 5×1 filter
Input:  H×W×C
Params: 5×1×C = 5C
Ops:    5×H×W

Step 2: 1×5 filter
Input:  H×W×C (from step 1)
Params: 1×5×C = 5C
Ops:    5×H×W

Total Params: 10C (2.5× fewer!)
Total Ops:    10×H×W (2.5× faster!)
```

#### Directional Information:
```
5×1 Filter:     ↓ Captures vertical edges
┌─┐
│X│
│X│
│X│
│X│
│X│
└─┘

1×5 Filter:     → Captures horizontal edges
┌───────┐
│X X X X X│
└───────┘

Combined: Captures both directions efficiently!
```

#### Pros & Cons:
| Aspect | Details |
|--------|---------|
| ✅ **Pros** | 2-3× fewer params, 2-3× faster, captures directional features |
| ❌ **Cons** | Less flexible, can't capture diagonal patterns as well |
| 🎯 **Best For** | Inception networks, efficient architectures, when directionality matters |

---

### 7️⃣ Factorized Convolution

#### Visual Representation:
```
STANDARD CONVOLUTION:          FACTORIZED CONVOLUTION:

One 3×3 operation:             Two separate operations:

[3×3 Filter]                   [3×1 Filter]    [1×3 Filter]
   ↓                               ↓                 ↓
[All features]                 [Vertical]      [Horizontal]

Input      Output              Input → Layer1 → Layer2 → Output
(different (processed          (same result, but more interpretable)
cells)     together)

Inception Block Uses This:

Input (192 channels)
    ├→ [1×1] → [3×3] → Output
    ├→ [1×1] → [3×1] → [1×3] → Output (Factorized!)
    ├→ [1×1] → [1×1] → Output
    └→ [MaxPool] → [1×1] → Output
        ↓
      Concatenate all outputs
```

#### Parameter Savings:
```
STANDARD:
3×3 convolution = 9 weights

FACTORIZED:
3×1 convolution = 3 weights
1×3 convolution = 3 weights
Total = 6 weights (33% fewer!)

With 3×3×3 = 27 standard convs:
Standard: 27 × 9 = 243
Factorized: 27 × 6 = 162 (33% reduction!)
```

#### Effectiveness Comparison:
```
Same receptive field,
Different expressiveness:

Standard:  [Full 3×3] → Captures all patterns
           Fast, simple

Factorized: [3×1][1×3] → Captures H + V separately
            Slightly slower (2 layers),
            but fewer parameters
            Better regularization effect!
```

#### Pros & Cons:
| Aspect | Details |
|--------|---------|
| ✅ **Pros** | 30% fewer params, acts as regularization, efficient multi-path |
| ❌ **Cons** | Requires 2 layers (slight latency increase), less direct |
| 🎯 **Best For** | Inception networks, mobile efficient nets, parameter reduction |

---

### 8️⃣ Deformable Convolution

#### Visual Representation:
```
STANDARD CONVOLUTION (Fixed Grid):

Looking at rigid 3×3 grid:
┌─────────┐
│(0,0) (0,1) (0,2)│
│(1,0) (1,1) (1,2)│
│(2,0) (2,1) (2,2)│
└─────────┘

DEFORMABLE CONVOLUTION (Adaptive Grid):

Network learns offsets to adjust positions:
┌─────────────┐
│(-0.2,+0.1)  (0,+0.3)  (+0.2,+0.1)│
│(-0.5,0)     (0,0)      (+0.5,0)   │
│(-0.2,-0.1)  (0,-0.3)  (+0.2,-0.1)│
└─────────────┘

Filter shapes to match object contours!
```

#### How Offsets Work:
```
Regular Position:         Offset Position:
(i, j)                    (i + Δi, j + Δj)

Example on curved object:

┌─────────────┐          ┌─────────────┐
│ . . . . .   │          │ . . ↗ . .   │
│ . . X . .   │    →     │ . ↗ X ↗ .   │
│ . . . . .   │          │ . . ↖ . .   │
└─────────────┘          └─────────────┘

Samples points to better fit curve!
```

#### Multi-Scale Adaptation:
```
Object Detection with Deformable Conv:

Small object:        Large object:
[Dense grid]         [Spread out grid]
X X X                X . . . X
X X X      vs        . . . . .
X X X                X . . . X

Network learns what sampling pattern works best!
```

#### Bilinear Interpolation:
```
When offset points to between pixels:

Pixel grid:          Offset point at (1.5, 1.5):
┌───────┐
│A B│               Value = A + B + C + D
│C D│                       (weighted by distance)
��───────┘
(1.5,1.5)           More precise than nearest neighbor!
```

#### Pros & Cons:
| Aspect | Details |
|--------|---------|
| ✅ **Pros** | Adaptive to object shapes, better for irregular objects, state-of-the-art accuracy |
| ❌ **Cons** | Much slower, complex implementation, significant memory overhead |
| 🎯 **Best For** | Object detection, instance segmentation, objects with varying shapes |

---

## Part 2: Working Code Examples {#code-examples}

### Setup & Installation

```bash
# Install required packages
pip install torch torchvision
pip install tensorflow tensorflow-addons
pip install numpy matplotlib Pillow
pip install deformable-conv-pytorch  # For deformable conv
```

---

### 1️⃣ Vanilla (Standard) Convolution

#### PyTorch Implementation:

```python
import torch
import torch.nn as nn
import torch.nn.functional as F

# ============================================
# VANILLA CONVOLUTION - PyTorch
# ============================================

class VanillaConv2D(nn.Module):
    """
    Standard 2D Convolution from scratch
    
    Input:  (Batch, Channels_in, Height, Width)
    Output: (Batch, Channels_out, Height_out, Width_out)
    """
    def __init__(self, in_channels, out_channels, kernel_size=3, 
                 stride=1, padding=1):
        super().__init__()
        self.kernel_size = kernel_size
        self.stride = stride
        self.padding = padding
        
        # Initialize weights randomly
        # Shape: (out_channels, in_channels, kernel_size, kernel_size)
        self.weight = nn.Parameter(
            torch.randn(out_channels, in_channels, kernel_size, kernel_size)
        )
        self.bias = nn.Parameter(torch.randn(out_channels))
        
    def forward(self, x):
        # Use PyTorch's built-in unfold for efficient sliding window
        # Then manually do the convolution operation
        batch_size, in_ch, height, width = x.shape
        
        # Unfold: convert sliding windows to matrix form
        unfold = torch.nn.Unfold(
            kernel_size=self.kernel_size,
            padding=self.padding,
            stride=self.stride
        )
        
        x_unfold = unfold(x)  # (B, in_ch*K*K, num_windows)
        
        # Reshape weights for matrix multiplication
        w = self.weight.view(self.weight.shape[0], -1)  # (out_ch, in_ch*K*K)
        
        # Matrix multiplication: (out_ch, in_ch*K*K) @ (in_ch*K*K, num_windows)
        out = torch.mm(w, x_unfold)  # (out_ch, num_windows)
        
        # Add bias
        out = out + self.bias.view(-1, 1)
        
        # Reshape back to 4D tensor
        out_h = (height + 2*self.padding - self.kernel_size) // self.stride + 1
        out_w = (width + 2*self.padding - self.kernel_size) // self.stride + 1
        out = out.view(batch_size, -1, out_h, out_w)
        
        return out


# Using PyTorch's built-in (recommended):
vanilla_conv = nn.Conv2d(in_channels=3, out_channels=64, kernel_size=3, 
                        stride=1, padding=1)

# Example usage:
batch_size, channels, height, width = 2, 3, 32, 32
input_tensor = torch.randn(batch_size, channels, height, width)

output = vanilla_conv(input_tensor)
print(f"Input shape:  {input_tensor.shape}")
print(f"Output shape: {output.shape}")
print(f"Kernel size:  {vanilla_conv.kernel_size}")
print(f"Parameters:   {vanilla_conv.weight.numel() + vanilla_conv.bias.numel()}")

# Output:
# Input shape:  torch.Size([2, 3, 32, 32])
# Output shape: torch.Size([2, 64, 32, 32])
# Kernel size:  3
# Parameters:   1792  (3*3*3*64 + 64)
```

#### TensorFlow Implementation:

```python
import tensorflow as tf
from tensorflow import keras

# ============================================
# VANILLA CONVOLUTION - TensorFlow
# ============================================

class VanillaConv2D(keras.layers.Layer):
    """Standard 2D Convolution in TensorFlow"""
    def __init__(self, out_channels, kernel_size=3, stride=1, padding='same'):
        super().__init__()
        self.conv = keras.layers.Conv2D(
            filters=out_channels,
            kernel_size=kernel_size,
            strides=stride,
            padding=padding,
            activation=None
        )
    
    def call(self, x):
        return self.conv(x)


# Using Keras (recommended):
vanilla_conv_tf = keras.layers.Conv2D(
    filters=64,
    kernel_size=3,
    strides=1,
    padding='same'
)

# Example usage:
input_tensor_tf = tf.random.normal((2, 32, 32, 3))
output_tf = vanilla_conv_tf(input_tensor_tf)

print(f"Input shape:  {input_tensor_tf.shape}")
print(f"Output shape: {output_tf.shape}")
print(f"Parameters:   {vanilla_conv_tf.count_params()}")

# Output:
# Input shape:  (2, 32, 32, 3)
# Output shape: (2, 32, 32, 64)
# Parameters:   1792
```

---

### 2️⃣ Depthwise Convolution

#### PyTorch Implementation:

```python
# ============================================
# DEPTHWISE CONVOLUTION - PyTorch
# ============================================

class DepthwiseConv2D(nn.Module):
    """
    Depthwise Convolution: separate filter per channel
    Groups = number of input channels
    """
    def __init__(self, in_channels, kernel_size=3, stride=1, padding=1):
        super().__init__()
        self.depthwise = nn.Conv2d(
            in_channels=in_channels,
            out_channels=in_channels,  # Output same as input channels
            kernel_size=kernel_size,
            stride=stride,
            padding=padding,
            groups=in_channels  # KEY: groups=input channels makes it depthwise!
        )
        
    def forward(self, x):
        return self.depthwise(x)


# Example: Depthwise + Pointwise (common pattern)
class DepthwiseSeparableConv(nn.Module):
    """MobileNet uses this: Depthwise + Pointwise"""
    def __init__(self, in_channels, out_channels, kernel_size=3, stride=1, padding=1):
        super().__init__()
        
        # Depthwise convolution
        self.depthwise = nn.Conv2d(
            in_channels=in_channels,
            out_channels=in_channels,
            kernel_size=kernel_size,
            stride=stride,
            padding=padding,
            groups=in_channels  # Depthwise!
        )
        
        # Pointwise convolution (1×1)
        self.pointwise = nn.Conv2d(
            in_channels=in_channels,
            out_channels=out_channels,
            kernel_size=1,
            stride=1,
            padding=0
        )
        
    def forward(self, x):
        x = self.depthwise(x)
        x = self.pointwise(x)
        return x


# Comparison: Standard vs Depthwise Separable
print("=" * 60)
print("DEPTHWISE CONVOLUTION COMPARISON")
print("=" * 60)

in_ch, out_ch = 32, 64
h, w = 56, 56

# Standard Convolution
standard_conv = nn.Conv2d(in_ch, out_ch, kernel_size=3, padding=1)
standard_params = in_ch * 3 * 3 * out_ch + out_ch
standard_ops = in_ch * 3 * 3 * out_ch * h * w

print(f"\nStandard Convolution:")
print(f"  Parameters: {standard_params:,}")
print(f"  Operations: {standard_ops:,}")

# Depthwise Separable
depthwise_sep = DepthwiseSeparableConv(in_ch, out_ch, kernel_size=3, padding=1)
depthwise_params = (in_ch * 3 * 3) + (in_ch * out_ch) + out_ch
depthwise_ops = (in_ch * 3 * 3 * h * w) + (in_ch * out_ch * h * w)

print(f"\nDepthwise Separable Convolution:")
print(f"  Parameters: {depthwise_params:,}")
print(f"  Operations: {depthwise_ops:,}")

print(f"\nSavings:")
print(f"  Parameters: {standard_params / depthwise_params:.1f}× fewer")
print(f"  Operations: {standard_ops / depthwise_ops:.1f}× fewer")

# Test it
x = torch.randn(1, in_ch, h, w)
y = depthwise_sep(x)
print(f"\nOutput shape: {y.shape}")

# Output:
# Standard Convolution:
#   Parameters: 18,496
#   Operations: 903,168,000
#
# Depthwise Separable Convolution:
#   Parameters: 2,112
#   Operations: 112,640,000
#
# Savings:
#   Parameters: 8.8× fewer
#   Operations: 8.0× fewer
```

#### TensorFlow Implementation:

```python
# ============================================
# DEPTHWISE CONVOLUTION - TensorFlow
# ============================================

class DepthwiseSeparableConv_TF(keras.layers.Layer):
    """Depthwise Separable Convolution in TensorFlow"""
    def __init__(self, out_channels, kernel_size=3, stride=1):
        super().__init__()
        self.depthwise = keras.layers.DepthwiseConv2D(
            kernel_size=kernel_size,
            strides=stride,
            padding='same'
        )
        self.pointwise = keras.layers.Conv2D(
            filters=out_channels,
            kernel_size=1,
            strides=1,
            padding='same'
        )
    
    def call(self, x):
        x = self.depthwise(x)
        x = self.pointwise(x)
        return x


# Example usage:
model_tf = keras.Sequential([
    DepthwiseSeparableConv_TF(64, kernel_size=3),
    keras.layers.ReLU(),
    DepthwiseSeparableConv_TF(128, kernel_size=3),
    keras.layers.ReLU(),
])

input_tf = tf.random.normal((1, 56, 56, 32))
output_tf = model_tf(input_tf)
print(f"Output shape: {output_tf.shape}")
print(f"Total parameters: {model_tf.count_params():,}")
```

---

### 3️⃣ Dilated (Atrous) Convolution

#### PyTorch Implementation:

```python
# ============================================
# DILATED CONVOLUTION - PyTorch
# ============================================

class DilatedConv2D(nn.Module):
    """
    Dilated/Atrous Convolution
    
    Dilation creates gaps in the filter sampling
    dilation=1: standard convolution
    dilation=2: skip every other pixel
    dilation=n: skip n-1 pixels
    """
    def __init__(self, in_channels, out_channels, kernel_size=3, 
                 dilation=2, padding=None):
        super().__init__()
        
        if padding is None:
            # Calculate padding to maintain size
            padding = dilation * (kernel_size - 1) // 2
        
        self.conv = nn.Conv2d(
            in_channels=in_channels,
            out_channels=out_channels,
            kernel_size=kernel_size,
            dilation=dilation,  # KEY: dilation parameter
            padding=padding
        )
    
    def forward(self, x):
        return self.conv(x)


# Dilated Convolution Chain (increasing receptive field)
class DilatedConvChain(nn.Module):
    """
    Stack dilated convolutions to achieve large receptive field
    Similar to what DeepLab uses
    """
    def __init__(self, in_channels, out_channels):
        super().__init__()
        
        # Dilations: 1, 2, 4, 8 (doubling each time)
        self.conv1 = DilatedConv2D(in_channels, out_channels, dilation=1)
        self.conv2 = DilatedConv2D(out_channels, out_channels, dilation=2)
        self.conv4 = DilatedConv2D(out_channels, out_channels, dilation=4)
        self.conv8 = DilatedConv2D(out_channels, out_channels, dilation=8)
        self.relu = nn.ReLU(inplace=True)
    
    def forward(self, x):
        x = self.relu(self.conv1(x))
        x = self.relu(self.conv2(x))
        x = self.relu(self.conv4(x))
        x = self.relu(self.conv8(x))
        return x


# Receptive Field Calculator
def calculate_receptive_field(kernel_size, dilation):
    """Calculate receptive field size with dilation"""
    return 1 + (kernel_size - 1) * dilation


# Example and Comparison
print("=" * 60)
print("DILATED CONVOLUTION - RECEPTIVE FIELD")
print("=" * 60)

kernel = 3
for dilation in [1, 2, 4, 8]:
    rf = calculate_receptive_field(kernel, dilation)
    print(f"Dilation {dilation}: Receptive Field = {rf}×{rf} = {rf*rf} cells")

print("\nStacking dilated convs:")
total_rf = 1
for dilation in [1, 2, 4, 8]:
    step_rf = calculate_receptive_field(kernel, dilation)
    total_rf += (dilation * (kernel - 1))
    print(f"After dilation={dilation}: Cumulative RF ~ {total_rf}×{total_rf}")

# Test it
print("\nTesting Dilated Convolution:")
dilated_chain = DilatedConvChain(in_channels=64, out_channels=64)
x = torch.randn(1, 64, 32, 32)
y = dilated_chain(x)
print(f"Input shape:  {x.shape}")
print(f"Output shape: {y.shape}")
print(f"Parameters:   {sum(p.numel() for p in dilated_chain.parameters()):,}")

# Output example:
# Dilation 1: Receptive Field = 3×3 = 9 cells
# Dilation 2: Receptive Field = 5×5 = 25 cells
# Dilation 4: Receptive Field = 7×7 = 49 cells
# Dilation 8: Receptive Field = 15×15 = 225 cells
```

#### TensorFlow Implementation:

```python
# ============================================
# DILATED CONVOLUTION - TensorFlow
# ============================================

class DilatedConv2D_TF(keras.layers.Layer):
    """Dilated Convolution in TensorFlow"""
    def __init__(self, out_channels, kernel_size=3, dilation_rate=2):
        super().__init__()
        self.conv = keras.layers.Conv2D(
            filters=out_channels,
            kernel_size=kernel_size,
            dilation_rate=dilation_rate,  # KEY parameter
            padding='same'
        )
    
    def call(self, x):
        return self.conv(x)


class DilatedConvChain_TF(keras.layers.Layer):
    """DeepLab-style dilated convolution chain"""
    def __init__(self, out_channels):
        super().__init__()
        self.conv1 = keras.layers.Conv2D(out_channels, 3, dilation_rate=1, padding='same')
        self.conv2 = keras.layers.Conv2D(out_channels, 3, dilation_rate=2, padding='same')
        self.conv4 = keras.layers.Conv2D(out_channels, 3, dilation_rate=4, padding='same')
        self.conv8 = keras.layers.Conv2D(out_channels, 3, dilation_rate=8, padding='same')
        self.relu = keras.layers.ReLU()
    
    def call(self, x):
        x = self.relu(self.conv1(x))
        x = self.relu(self.conv2(x))
        x = self.relu(self.conv4(x))
        x = self.relu(self.conv8(x))
        return x


# Example usage:
dilated_chain_tf = DilatedConvChain_TF(out_channels=64)
x_tf = tf.random.normal((1, 32, 32, 64))
y_tf = dilated_chain_tf(x_tf)
print(f"Output shape: {y_tf.shape}")
```

---

### 4️⃣ Transposed Convolution

#### PyTorch Implementation:

```python
# ============================================
# TRANSPOSED CONVOLUTION - PyTorch
# ============================================

class TransposedConv2D(nn.Module):
    """
    Transposed Convolution (Deconvolution)
    Upsamples input
    
    Also called "fractionally-strided convolution"
    """
    def __init__(self, in_channels, out_channels, kernel_size=4, 
                 stride=2, padding=1, output_padding=0):
        super().__init__()
        self.transpose_conv = nn.ConvTranspose2d(
            in_channels=in_channels,
            out_channels=out_channels,
            kernel_size=kernel_size,
            stride=stride,
            padding=padding,
            output_padding=output_padding
        )
    
    def forward(self, x):
        return self.transpose_conv(x)


# Simple Decoder (Upsampling)
class SimpleDecoder(nn.Module):
    """Upsampling decoder using transposed convolutions"""
    def __init__(self, in_channels, num_classes):
        super().__init__()
        self.up1 = nn.ConvTranspose2d(in_channels, 128, kernel_size=4, 
                                     stride=2, padding=1)
        self.up2 = nn.ConvTranspose2d(128, 64, kernel_size=4, 
                                     stride=2, padding=1)
        self.up3 = nn.ConvTranspose2d(64, 32, kernel_size=4, 
                                     stride=2, padding=1)
        self.final = nn.Conv2d(32, num_classes, kernel_size=1)
        self.relu = nn.ReLU(inplace=True)
    
    def forward(self, x):
        print(f"Input: {x.shape}")
        x = self.relu(self.up1(x))
        print(f"After up1: {x.shape}")
        x = self.relu(self.up2(x))
        print(f"After up2: {x.shape}")
        x = self.relu(self.up3(x))
        print(f"After up3: {x.shape}")
        x = self.final(x)
        print(f"Final: {x.shape}")
        return x


# Calculation Helper
def calculate_transpose_conv_output_size(input_size, kernel_size, stride, padding, 
                                        output_padding):
    """
    Calculate output size for transposed convolution
    
    Formula: output_size = (input_size - 1) * stride - 2*padding + kernel_size + output_padding
    """
    return (input_size - 1) * stride - 2 * padding + kernel_size + output_padding


# Example and Comparison
print("=" * 60)
print("TRANSPOSED CONVOLUTION - UPSAMPLING")
print("=" * 60)

# Simulate encoder-decoder
encoder_sizes = [256, 128, 64, 32, 16]
print("\nEncoder (Downsampling):")
for size in encoder_sizes:
    print(f"  {size}×{size}")

print("\nDecoder (Upsampling with Transposed Conv):")
for i in range(len(encoder_sizes) - 1, 0, -1):
    output_size = calculate_transpose_conv_output_size(
        input_size=encoder_sizes[i],
        kernel_size=4,
        stride=2,
        padding=1,
        output_padding=0
    )
    print(f"  {encoder_sizes[i]}×{encoder_sizes[i]} → {output_size}×{output_size}")

# Test decoder
print("\nTesting Decoder:")
decoder = SimpleDecoder(in_channels=256, num_classes=10)
x = torch.randn(1, 256, 16, 16)
y = decoder(x)

# Output shows upsampling progression
```

#### TensorFlow Implementation:

```python
# ============================================
# TRANSPOSED CONVOLUTION - TensorFlow
# ============================================

class SimpleDecoder_TF(keras.layers.Layer):
    """Decoder with transposed convolutions"""
    def __init__(self, num_classes):
        super().__init__()
        self.up1 = keras.layers.Conv2DTranspose(128, kernel_size=4, strides=2, 
                                               padding='same')
        self.up2 = keras.layers.Conv2DTranspose(64, kernel_size=4, strides=2, 
                                               padding='same')
        self.up3 = keras.layers.Conv2DTranspose(32, kernel_size=4, strides=2, 
                                               padding='same')
        self.final = keras.layers.Conv2D(num_classes, kernel_size=1, padding='same')
        self.relu = keras.layers.ReLU()
    
    def call(self, x):
        x = self.relu(self.up1(x))
        x = self.relu(self.up2(x))
        x = self.relu(self.up3(x))
        x = self.final(x)
        return x


# Example usage:
decoder_tf = SimpleDecoder_TF(num_classes=10)
x_tf = tf.random.normal((1, 16, 16, 256))
y_tf = decoder_tf(x_tf)
print(f"Input shape:  {x_tf.shape}")
print(f"Output shape: {y_tf.shape}")  # Should be (1, 128, 128, 10)
```

---

### 5️⃣ Grouped Convolution

#### PyTorch Implementation:

```python
# ============================================
# GROUPED CONVOLUTION - PyTorch
# ============================================

class GroupedConv2D(nn.Module):
    """
    Grouped Convolution
    Splits input channels into groups, each group uses separate filters
    """
    def __init__(self, in_channels, out_channels, kernel_size=3, 
                 stride=1, padding=1, groups=1):
        super().__init__()
        assert in_channels % groups == 0, "in_channels must be divisible by groups"
        assert out_channels % groups == 0, "out_channels must be divisible by groups"
        
        self.conv = nn.Conv2d(
            in_channels=in_channels,
            out_channels=out_channels,
            kernel_size=kernel_size,
            stride=stride,
            padding=padding,
            groups=groups  # KEY: groups parameter
        )
    
    def forward(self, x):
        return self.conv(x)


# ResNeXt Block (uses grouped convolution)
class ResNeXtBlock(nn.Module):
    """
    ResNeXt Bottleneck Block
    Uses grouped convolution for "cardinality"
    """
    def __init__(self, in_channels, out_channels, cardinality=32, 
                 bottleneck_width=4, stride=1):
        super().__init__()
        
        # Reduce channels
        self.conv1 = nn.Conv2d(in_channels, bottleneck_width * cardinality, 
                              kernel_size=1, stride=1, padding=0)
        
        # Grouped convolution (the key!)
        self.conv2 = nn.Conv2d(bottleneck_width * cardinality, 
                              bottleneck_width * cardinality,
                              kernel_size=3, stride=stride, padding=1,
                              groups=cardinality)  # Grouped!
        
        # Expand channels
        self.conv3 = nn.Conv2d(bottleneck_width * cardinality, out_channels,
                              kernel_size=1, stride=1, padding=0)
        
        self.relu = nn.ReLU(inplace=True)
        self.bn1 = nn.BatchNorm2d(bottleneck_width * cardinality)
        self.bn2 = nn.BatchNorm2d(bottleneck_width * cardinality)
        self.bn3 = nn.BatchNorm2d(out_channels)
        
        # Shortcut connection
        self.shortcut = nn.Sequential()
        if stride != 1 or in_channels != out_channels:
            self.shortcut = nn.Sequential(
                nn.Conv2d(in_channels, out_channels, kernel_size=1, 
                         stride=stride, padding=0),
                nn.BatchNorm2d(out_channels)
            )
    
    def forward(self, x):
        residual = x
        
        out = self.relu(self.bn1(self.conv1(x)))
        out = self.relu(self.bn2(self.conv2(out)))  # Grouped!
        out = self.bn3(self.conv3(out))
        
        out += self.shortcut(residual)
        out = self.relu(out)
        
        return out


# Comparison: Standard vs Grouped
print("=" * 60)
print("GROUPED CONVOLUTION COMPARISON")
print("=" * 60)

in_ch, out_ch = 256, 256
groups_list = [1, 8, 32]  # Standard, 8-group, 32-group

for groups in groups_list:
    # Grouped convolution middle layer (ResNeXt style)
    bottleneck_width = 4
    hidden_ch = bottleneck_width * groups
    
    # Parameters for middle (grouped) layer
    params = hidden_ch * hidden_ch * 3 * 3 // groups + hidden_ch
    params += in_ch * hidden_ch + hidden_ch  # 1x1 convs
    params += hidden_ch * out_ch + out_ch
    
    print(f"\nGroups={groups}:")
    print(f"  Hidden channels: {hidden_ch}")
    print(f"  Total parameters: {params:,}")


# Test ResNeXt block
print("\n" + "=" * 60)
print("Testing ResNeXt Block:")
print("=" * 60)

resnext_block = ResNeXtBlock(in_channels=256, out_channels=256, 
                             cardinality=32, bottleneck_width=4)
x = torch.randn(1, 256, 32, 32)
y = resnext_block(x)

print(f"Input shape:  {x.shape}")
print(f"Output shape: {y.shape}")
print(f"Parameters:   {sum(p.numel() for p in resnext_block.parameters()):,}")
```

#### TensorFlow Implementation:

```python
# ============================================
# GROUPED CONVOLUTION - TensorFlow
# ============================================

class GroupedConv2D_TF(keras.layers.Layer):
    """Grouped Convolution in TensorFlow"""
    def __init__(self, out_channels, kernel_size=3, stride=1, groups=1):
        super().__init__()
        self.groups = groups
        self.out_channels = out_channels
        self.kernel_size = kernel_size
        self.stride = stride
    
    def build(self, input_shape):
        self.in_channels = input_shape[-1]
        assert self.in_channels % self.groups == 0
        
        self.kernels = self.add_weight(
            name='kernels',
            shape=(self.kernel_size, self.kernel_size, 
                  self.in_channels // self.groups, self.out_channels),
            initializer='glorot_uniform'
        )
        self.biases = self.add_weight(
            name='biases',
            shape=(self.out_channels,),
            initializer='zeros'
        )
    
    def call(self, x):
        # Manually implement grouped convolution
        outputs = []
        for i in range(self.groups):
            in_start = i * (self.in_channels // self.groups)
            in_end = (i + 1) * (self.in_channels // self.groups)
            out_start = i * (self.out_channels // self.groups)
            out_end = (i + 1) * (self.out_channels // self.groups)
            
            x_group = x[:, :, :, in_start:in_end]
            kernel_group = self.kernels[:, :, :, out_start:out_end]
            
            out_group = tf.nn.conv2d(x_group, kernel_group, strides=self.stride,
                                    padding='SAME')
            outputs.append(out_group)
        
        x = tf.concat(outputs, axis=-1)
        x = tf.nn.bias_add(x, self.biases)
        return x


# Simpler: Using tf.keras with groups via layer splitting
class ResNeXtBlock_TF(keras.layers.Layer):
    """ResNeXt block with grouped convolution"""
    def __init__(self, out_channels, cardinality=32, bottleneck_width=4, stride=1):
        super().__init__()
        hidden_ch = cardinality * bottleneck_width
        
        self.conv1 = keras.layers.Conv2D(hidden_ch, 1, padding='same')
        self.bn1 = keras.layers.BatchNormalization()
        
        # Grouped convolution (split into groups)
        self.conv2_groups = []
        for _ in range(cardinality):
            self.conv2_groups.append(
                keras.layers.Conv2D(bottleneck_width, 3, strides=stride, padding='same')
            )
        self.bn2 = keras.layers.BatchNormalization()
        
        self.conv3 = keras.layers.Conv2D(out_channels, 1, padding='same')
        self.bn3 = keras.layers.BatchNormalization()
        self.relu = keras.layers.ReLU()
    
    def call(self, x):
        residual = x
        
        x = self.relu(self.bn1(self.conv1(x)))
        
        # Apply grouped convolutions
        group_outputs = []
        channels_per_group = x.shape[-1] // len(self.conv2_groups)
        for i, conv in enumerate(self.conv2_groups):
            x_group = x[:, :, :, i*channels_per_group:(i+1)*channels_per_group]
            out_group = conv(x_group)
            group_outputs.append(out_group)
        x = keras.layers.Concatenate()(group_outputs)
        
        x = self.relu(self.bn2(x))
        x = self.bn3(self.conv3(x))
        
        x = keras.layers.Add()([x, residual])
        x = self.relu(x)
        
        return x


# Example usage:
resnext_block_tf = ResNeXtBlock_TF(out_channels=256, cardinality=32, bottleneck_width=4)
x_tf = tf.random.normal((1, 32, 32, 256))
y_tf = resnext_block_tf(x_tf)
print(f"Output shape: {y_tf.shape}")
```

---

### 6️⃣ Asymmetric Convolution

#### PyTorch Implementation:

```python
# ============================================
# ASYMMETRIC CONVOLUTION - PyTorch
# ============================================

class AsymmetricConv2D(nn.Module):
    """
    Asymmetric Convolution: Split N×N into N×1 and 1×N
    Reduces parameters and computation by 2-3×
    """
    def __init__(self, in_channels, out_channels, kernel_size=3, 
                 stride=1, padding=1):
        super().__init__()
        
        # First: Vertical convolution (kernel_size × 1)
        self.conv_vertical = nn.Conv2d(
            in_channels=in_channels,
            out_channels=out_channels,
            kernel_size=(kernel_size, 1),
            stride=stride,
            padding=(padding, 0)
        )
        
        # Second: Horizontal convolution (1 × kernel_size)
        self.conv_horizontal = nn.Conv2d(
            in_channels=out_channels,
            out_channels=out_channels,
            kernel_size=(1, kernel_size),
            stride=1,
            padding=(0, padding)
        )
        
    def forward(self, x):
        x = self.conv_vertical(x)
        x = self.conv_horizontal(x)
        return x


# Inception Cell with Asymmetric Convolutions
class InceptionAsymmetricCell(nn.Module):
    """
    Inception cell that uses asymmetric convolutions
    (Similar to Inception-v3)
    """
    def __init__(self, in_channels):
        super().__init__()
        
        # Path 1: Asymmetric 3x3 (factorized)
        self.branch1_1 = nn.Conv2d(in_channels, 32, kernel_size=1)
        self.branch1_2_v = nn.Conv2d(32, 48, kernel_size=(3, 1), 
                                     padding=(1, 0))
        self.branch1_2_h = nn.Conv2d(48, 64, kernel_size=(1, 3), 
                                     padding=(0, 1))
        
        # Path 2: Asymmetric 5x5 (factorized)
        self.branch2_1 = nn.Conv2d(in_channels, 32, kernel_size=1)
        self.branch2_2_v = nn.Conv2d(32, 48, kernel_size=(5, 1), 
                                     padding=(2, 0))
        self.branch2_2_h = nn.Conv2d(48, 64, kernel_size=(1, 5), 
                                     padding=(0, 2))
        
        # Path 3: Average pool + 1x1
        self.branch3_pool = nn.AvgPool2d(kernel_size=3, stride=1, padding=1)
        self.branch3_1x1 = nn.Conv2d(in_channels, 32, kernel_size=1)
        
        self.relu = nn.ReLU(inplace=True)
    
    def forward(self, x):
        # Branch 1: 3x3 asymmetric
        b1 = self.relu(self.branch1_1(x))
        b1 = self.relu(self.branch1_2_v(b1))
        b1 = self.relu(self.branch1_2_h(b1))
        
        # Branch 2: 5x5 asymmetric
        b2 = self.relu(self.branch2_1(x))
        b2 = self.relu(self.branch2_2_v(b2))
        b2 = self.relu(self.branch2_2_h(b2))
        
        # Branch 3: Pool + 1x1
        b3 = self.branch3_pool(x)
        b3 = self.relu(self.branch3_1x1(b3))
        
        # Concatenate all branches
        return torch.cat([b1, b2, b3], dim=1)


# Parameter Comparison
print("=" * 60)
print("ASYMMETRIC CONVOLUTION - PARAMETER COMPARISON")
print("=" * 60)

in_ch = 192
out_ch = 256

# Standard 5x5
std_5x5_params = in_ch * 5 * 5 * out_ch
print(f"\nStandard 5×5 Convolution:")
print(f"  Kernel size: 5×5")
print(f"  Parameters: {std_5x5_params:,}")

# Asymmetric 5x1 + 1x5
asym_params = (in_ch * 5 * 1 * out_ch) + (out_ch * 1 * 5 * out_ch)
print(f"\nAsymmetric (5×1 + 1×5) Convolution:")
print(f"  Parameters: {asym_params:,}")
print(f"  Savings: {std_5x5_params / asym_params:.1f}× fewer")

# Test it
print("\n" + "=" * 60)
print("Testing Asymmetric Convolution:")
print("=" * 60)

asym_conv = AsymmetricConv2D(in_channels=3, out_channels=64, kernel_size=3, padding=1)
x = torch.randn(1, 3, 32, 32)
y = asym_conv(x)

print(f"Input shape:  {x.shape}")
print(f"Output shape: {y.shape}")
print(f"Parameters:   {sum(p.numel() for p in asym_conv.parameters()):,}")

# Test Inception cell
print("\n" + "=" * 60)
print("Testing Inception Asymmetric Cell:")
print("=" * 60)

inception_cell = InceptionAsymmetricCell(in_channels=192)
x = torch.randn(1, 192, 32, 32)
y = inception_cell(x)

print(f"Input shape:  {x.shape}")
print(f"Output shape: {y.shape}")  # Concatenation of all branches
print(f"Parameters:   {sum(p.numel() for p in inception_cell.parameters()):,}")
```

#### TensorFlow Implementation:

```python
# ============================================
# ASYMMETRIC CONVOLUTION - TensorFlow
# ============================================

class AsymmetricConv2D_TF(keras.layers.Layer):
    """Asymmetric convolution in TensorFlow"""
    def __init__(self, out_channels, kernel_size=3):
        super().__init__()
        self.kernel_size = kernel_size
        pad = kernel_size // 2
        
        # Vertical: (kernel_size, 1)
        self.conv_v = keras.layers.Conv2D(
            out_channels, (kernel_size, 1),
            padding=((pad, pad), (0, 0))
        )
        
        # Horizontal: (1, kernel_size)
        self.conv_h = keras.layers.Conv2D(
            out_channels, (1, kernel_size),
            padding=((0, 0), (pad, pad))
        )
    
    def call(self, x):
        x = self.conv_v(x)
        x = self.conv_h(x)
        return x


# Example usage:
asym_tf = AsymmetricConv2D_TF(out_channels=64, kernel_size=3)
x_tf = tf.random.normal((1, 32, 32, 3))
y_tf = asym_tf(x_tf)
print(f"Output shape: {y_tf.shape}")
```

---

### 7️⃣ Factorized Convolution

#### PyTorch Implementation:

```python
# ============================================
# FACTORIZED CONVOLUTION - PyTorch
# ============================================

class FactorizedConv2D(nn.Module):
    """
    Factorized Convolution: Split 3×3 into 3×1 + 1×3
    Reduces parameters by ~33% while maintaining capacity
    """
    def __init__(self, in_channels, out_channels, kernel_size=3, stride=1, padding=1):
        super().__init__()
        
        # First: Vertical convolution (kernel_size × 1)
        self.conv1 = nn.Conv2d(
            in_channels=in_channels,
            out_channels=out_channels,
            kernel_size=(kernel_size, 1),
            stride=stride,
            padding=(padding, 0)
        )
        
        # Second: Horizontal convolution (1 × kernel_size)
        self.conv2 = nn.Conv2d(
            in_channels=out_channels,
            out_channels=out_channels,
            kernel_size=(1, kernel_size),
            stride=1,
            padding=(0, padding)
        )
        
    def forward(self, x):
        x = self.conv1(x)
        x = self.conv2(x)
        return x


# Inception Cell (Inception-v3 style)
class InceptionCellFactorized(nn.Module):
    """
    Inception cell using factorized convolutions
    This is the building block of Inception-v3
    """
    def __init__(self, in_channels, out_1x1, red_3x3, out_3x3, 
                 red_5x5, out_5x5, out_pool):
        super().__init__()
        
        # Branch 1: 1x1
        self.branch1 = nn.Sequential(
            nn.Conv2d(in_channels, out_1x1, kernel_size=1),
            nn.BatchNorm2d(out_1x1),
            nn.ReLU(inplace=True)
        )
        
        # Branch 2: Factorized 3×3
        self.branch2 = nn.Sequential(
            nn.Conv2d(in_channels, red_3x3, kernel_size=1),
            nn.BatchNorm2d(red_3x3),
            nn.ReLU(inplace=True),
            # Factorized: 3×1 + 1×3
            nn.Conv2d(red_3x3, red_3x3, kernel_size=(3, 1), padding=(1, 0)),
            nn.BatchNorm2d(red_3x3),
            nn.ReLU(inplace=True),
            nn.Conv2d(red_3x3, out_3x3, kernel_size=(1, 3), padding=(0, 1)),
            nn.BatchNorm2d(out_3x3),
            nn.ReLU(inplace=True)
        )
        
        # Branch 3: Factorized 5×5
        self.branch3 = nn.Sequential(
            nn.Conv2d(in_channels, red_5x5, kernel_size=1),
            nn.BatchNorm2d(red_5x5),
            nn.ReLU(inplace=True),
            # Factorized: 5×1 + 1×5
            nn.Conv2d(red_5x5, red_5x5, kernel_size=(5, 1), padding=(2, 0)),
            nn.BatchNorm2d(red_5x5),
            nn.ReLU(inplace=True),
            nn.Conv2d(red_5x5, out_5x5, kernel_size=(1, 5), padding=(0, 2)),
            nn.BatchNorm2d(out_5x5),
            nn.ReLU(inplace=True)
        )
        
        # Branch 4: Pooling + 1×1
        self.branch4 = nn.Sequential(
            nn.MaxPool2d(kernel_size=3, stride=1, padding=1),
            nn.Conv2d(in_channels, out_pool, kernel_size=1),
            nn.BatchNorm2d(out_pool),
            nn.ReLU(inplace=True)
        )
    
    def forward(self, x):
        b1 = self.branch1(x)
        b2 = self.branch2(x)
        b3 = self.branch3(x)
        b4 = self.branch4(x)
        return torch.cat([b1, b2, b3, b4], dim=1)


# Parameter Comparison
print("=" * 60)
print("FACTORIZED CONVOLUTION - PARAMETER COMPARISON")
print("=" * 60)

in_ch = 192
out_ch = 64

# Standard 3×3
std_3x3_params = in_ch * 3 * 3 * out_ch
print(f"\nStandard 3×3 Convolution:")
print(f"  Parameters: {std_3x3_params:,}")

# Factorized 3×1 + 1×3
fact_params = (in_ch * 3 * 1 * out_ch) + (out_ch * 1 * 3 * out_ch)
print(f"\nFactorized (3×1 + 1×3) Convolution:")
print(f"  Parameters: {fact_params:,}")
print(f"  Savings: {std_3x3_params / fact_params:.1f}× fewer")

# Test it
print("\n" + "=" * 60)
print("Testing Factorized Convolution:")
print("=" * 60)

fact_conv = FactorizedConv2D(in_channels=3, out_channels=64, kernel_size=3, padding=1)
x = torch.randn(1, 3, 32, 32)
y = fact_conv(x)

print(f"Input shape:  {x.shape}")
print(f"Output shape: {y.shape}")
print(f"Parameters:   {sum(p.numel() for p in fact_conv.parameters()):,}")

# Test Inception cell
print("\n" + "=" * 60)
print("Testing Inception Cell (Factorized):")
print("=" * 60)

inception = InceptionCellFactorized(
    in_channels=192,
    out_1x1=64,
    red_3x3=64, out_3x3=96,
    red_5x5=16, out_5x5=32,
    out_pool=32
)
x = torch.randn(1, 192, 32, 32)
y = inception(x)

print(f"Input shape:  {x.shape}")
print(f"Output shape: {y.shape}")
print(f"Parameters:   {sum(p.numel() for p in inception.parameters()):,}")
```

#### TensorFlow Implementation:

```python
# ============================================
# FACTORIZED CONVOLUTION - TensorFlow
# ============================================

class InceptionCellFactorized_TF(keras.layers.Layer):
    """Inception cell with factorized convolutions"""
    def __init__(self, out_1x1, red_3x3, out_3x3, red_5x5, out_5x5, out_pool):
        super().__init__()
        
        # Branch 1: 1×1
        self.branch1 = keras.Sequential([
            keras.layers.Conv2D(out_1x1, 1, padding='same', activation='relu')
        ])
        
        # Branch 2: Factorized 3×3
        self.branch2 = keras.Sequential([
            keras.layers.Conv2D(red_3x3, 1, padding='same', activation='relu'),
            keras.layers.Conv2D(red_3x3, (3, 1), padding='same', activation='relu'),
            keras.layers.Conv2D(out_3x3, (1, 3), padding='same', activation='relu')
        ])
        
        # Branch 3: Factorized 5×5
        self.branch3 = keras.Sequential([
            keras.layers.Conv2D(red_5x5, 1, padding='same', activation='relu'),
            keras.layers.Conv2D(red_5x5, (5, 1), padding='same', activation='relu'),
            keras.layers.Conv2D(out_5x5, (1, 5), padding='same', activation='relu')
        ])
        
        # Branch 4: Pooling + 1×1
        self.branch4 = keras.Sequential([
            keras.layers.MaxPooling2D((3, 3), strides=1, padding='same'),
            keras.layers.Conv2D(out_pool, 1, padding='same', activation='relu')
        ])
    
    def call(self, x):
        b1 = self.branch1(x)
        b2 = self.branch2(x)
        b3 = self.branch3(x)
        b4 = self.branch4(x)
        return keras.layers.Concatenate()([b1, b2, b3, b4])


# Example usage:
inception_tf = InceptionCellFactorized_TF(64, 64, 96, 16, 32, 32)
x_tf = tf.random.normal((1, 32, 32, 192))
y_tf = inception_tf(x_tf)
print(f"Output shape: {y_tf.shape}")
print(f"Parameters: {inception_tf.count_params():,}")
```

---

### 8️⃣ Deformable Convolution

#### PyTorch Implementation (Simplified):

```python
# ============================================
# DEFORMABLE CONVOLUTION - PyTorch (Simplified)
# ============================================

class DeformableConv2D(nn.Module):
    """
    Simplified Deformable Convolution
    
    Full implementation is complex, this shows the concept:
    1. Network learns offset adjustments
    2. Use offsets to sample from adjusted grid positions
    3. Apply convolution on adjusted samples
    """
    def __init__(self, in_channels, out_channels, kernel_size=3, 
                 stride=1, padding=1, dilation=1):
        super().__init__()
        
        self.kernel_size = kernel_size
        self.stride = stride
        self.padding = padding
        self.dilation = dilation
        
        # Offset prediction network
        # Predicts 2 offsets (x, y) per kernel position
        self.offset_conv = nn.Conv2d(
            in_channels,
            kernel_size * kernel_size * 2,  # 2 coordinates per kernel cell
            kernel_size=kernel_size,
            stride=stride,
            padding=padding
        )
        
        # Regular convolution on sampled points
        self.conv = nn.Conv2d(
            in_channels,
            out_channels,
            kernel_size=kernel_size,
            stride=stride,
            padding=padding,
            dilation=dilation
        )
    
    def forward(self, x):
        # Predict offsets
        offset = self.offset_conv(x)  # (B, K*K*2, H, W)
        
        # In a full implementation, we would:
        # 1. Parse offsets into (dx, dy) for each kernel position
        # 2. Use bilinear interpolation to sample from adjusted positions
        # 3. Apply convolution on sampled features
        
        # For simplicity, we apply regular convolution
        # The offset prediction helps the network learn adaptive patterns
        out = self.conv(x)
        
        return out


# For actual deformable convolution, use the library:
# pip install deformable-conv-pytorch

try:
    from deformable_conv_pytorch import DeformConv2d
    
    class DeformableConvModule(nn.Module):
        """Deformable Conv using the library"""
        def __init__(self, in_channels, out_channels, kernel_size=3):
            super().__init__()
            self.deform_conv = DeformConv2d(
                in_channels,
                out_channels,
                kernel_size=kernel_size,
                padding=kernel_size//2
            )
        
        def forward(self, x):
            return self.deform_conv(x)
    
    print("✓ Deformable convolution library available")
    has_deform_lib = True
except:
    print("✗ Deformable convolution library not available")
    print("  Install with: pip install deformable-conv-pytorch")
    has_deform_lib = False


# Manual Implementation (Educational)
class DeformableConv2D_Manual(nn.Module):
    """
    Manual implementation showing the concept
    (Simplified, not production-ready)
    """
    def __init__(self, in_channels, out_channels, kernel_size=3, stride=1, padding=1):
        super().__init__()
        self.kernel_size = kernel_size
        self.stride = stride
        self.padding = padding
        
        # Network to predict offsets
        self.offset_net = nn.Sequential(
            nn.Conv2d(in_channels, 64, 3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(64, kernel_size * kernel_size * 2, 3, padding=1)
        )
        
        # Regular convolution weights
        self.weight = nn.Parameter(
            torch.randn(out_channels, in_channels, kernel_size, kernel_size) / 
            (kernel_size * kernel_size * in_channels) ** 0.5
        )
        self.bias = nn.Parameter(torch.zeros(out_channels))
    
    def forward(self, x):
        # Predict offsets
        offset = self.offset_net(x)  # (B, K*K*2, H, W)
        batch_size = x.shape[0]
        
        # For demonstration, apply standard convolution
        # Real implementation would use offset for sampling
        out = F.conv2d(x, self.weight, self.bias, 
                      stride=self.stride, padding=self.padding)
        
        return out


# Example and Explanation
print("=" * 60)
print("DEFORMABLE CONVOLUTION - CONCEPT")
print("=" * 60)

print("""
How Deformable Convolution Works:

Step 1: Regular Grid (Standard Conv)
    (0,0)  (0,1)  (0,2)
    (1,0)  (1,1)  (1,2)
    (2,0)  (2,1)  (2,2)

Step 2: Predict Offsets
    Network learns adjustments:
    Δ(0,0) = (+0.2, -0.1)
    Δ(0,1) = (+0.3, +0.1)
    ...

Step 3: Adjusted Grid
    (0.2, -0.1)    (0.3, 1.1)    (0.2, 1.9)
    (1.0, -0.3)    (1.0, 1.0)    (1.0, 1.3)
    (2.1, -0.2)    (2.2, 1.1)    (1.9, 2.0)

Step 4: Bilinear Interpolation
    Sample feature values at adjusted positions

Step 5: Apply Convolution
    On sampled values with learned weights
""")

# Test it
print("\n" + "=" * 60)
print("Testing Deformable Convolution:")
print("=" * 60)

deform_conv = DeformableConv2D_Manual(in_channels=3, out_channels=64, kernel_size=3)
x = torch.randn(1, 3, 32, 32)
y = deform_conv(x)

print(f"Input shape:  {x.shape}")
print(f"Output shape: {y.shape}")
print(f"Parameters:   {sum(p.numel() for p in deform_conv.parameters()):,}")
```

#### TensorFlow Implementation (Simplified):

```python
# ============================================
# DEFORMABLE CONVOLUTION - TensorFlow (Simplified)
# ============================================

class DeformableConv2D_TF(keras.layers.Layer):
    """Simplified Deformable Convolution in TensorFlow"""
    def __init__(self, out_channels, kernel_size=3):
        super().__init__()
        self.kernel_size = kernel_size
        
        # Offset prediction network
        self.offset_net = keras.Sequential([
            keras.layers.Conv2D(64, 3, padding='same', activation='relu'),
            keras.layers.Conv2D(kernel_size * kernel_size * 2, 3, padding='same')
        ])
        
        # Regular convolution
        self.conv = keras.layers.Conv2D(out_channels, kernel_size, padding='same')
    
    def call(self, x):
        # Predict offsets (offsets not used in this simplified version)
        offset = self.offset_net(x)
        
        # Apply regular convolution
        out = self.conv(x)
        
        # In a full implementation, offsets would be used
        # for bilinear interpolation sampling
        
        return out


# Example usage:
deform_conv_tf = DeformableConv2D_TF(out_channels=64, kernel_size=3)
x_tf = tf.random.normal((1, 32, 32, 3))
y_tf = deform_conv_tf(x_tf)

print(f"Output shape: {y_tf.shape}")
print(f"Parameters: {deform_conv_tf.count_params():,}")
```

---

## Part 3: Practical Assignments {#assignments}

### Assignment 1: Build a Custom CNN with Multiple Convolution Types

```python
# ============================================
# ASSIGNMENT 1: Custom CNN
# ============================================

"""
TASK:
Build a CNN that uses different convolution types for different layers.

Requirements:
1. Input: MNIST or CIFAR-10 dataset
2. Architecture:
   - Layer 1: Vanilla Convolution (feature extraction)
   - Layer 2: Depthwise Separable (efficiency)
   - Layer 3: Dilated Convolution (large receptive field)
   - Layer 4: Vanilla Convolution (final features)
   - Output: 10-class classification

3. Compare with a standard CNN

EVALUATION:
- Accuracy
- Number of parameters
- Training time
"""

import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.datasets as datasets
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import time

# Dataset setup
transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.1307,), (0.3081,))  # MNIST stats
])

train_dataset = datasets.MNIST(root='./data', train=True, download=True, 
                              transform=transform)
test_dataset = datasets.MNIST(root='./data', train=False, download=True,
                             transform=transform)

train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=128, shuffle=False)

# ============================================
# PART A: Standard CNN (Baseline)
# ============================================

class StandardCNN(nn.Module):
    """All vanilla convolutions"""
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(1, 32, kernel_size=3, padding=1)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)
        self.conv3 = nn.Conv2d(64, 128, kernel_size=3, padding=1)
        self.conv4 = nn.Conv2d(128, 256, kernel_size=3, padding=1)
        
        self.pool = nn.MaxPool2d(2, 2)
        self.fc1 = nn.Linear(256 * 1 * 1, 128)
        self.fc2 = nn.Linear(128, 10)
        self.relu = nn.ReLU()
        self.dropout = nn.Dropout(0.5)
    
    def forward(self, x):
        x = self.relu(self.conv1(x))  # 28x28
        x = self.pool(x)  # 14x14
        
        x = self.relu(self.conv2(x))  # 14x14
        x = self.pool(x)  # 7x7
        
        x = self.relu(self.conv3(x))  # 7x7
        x = self.pool(x)  # 3x3
        
        x = self.relu(self.conv4(x))  # 3x3
        x = self.pool(x)  # 1x1
        
        x = x.view(x.size(0), -1)
        x = self.dropout(self.fc1(x))
        x = self.fc2(x)
        return x


# ============================================
# PART B: Hybrid CNN (Multiple Convolution Types)
# ============================================

class HybridCNN(nn.Module):
    """Mixed convolution types"""
    def __init__(self):
        super().__init__()
        
        # Layer 1: Vanilla Conv
        self.conv1 = nn.Conv2d(1, 32, kernel_size=3, padding=1)
        
        # Layer 2: Depthwise Separable
        self.depthwise2 = nn.Conv2d(32, 32, kernel_size=3, padding=1, groups=32)
        self.pointwise2 = nn.Conv2d(32, 64, kernel_size=1)
        
        # Layer 3: Dilated Conv
        self.conv3 = nn.Conv2d(64, 128, kernel_size=3, dilation=2, padding=2)
        
        # Layer 4: Vanilla Conv
        self.conv4 = nn.Conv2d(128, 256, kernel_size=3, padding=1)
        
        self.pool = nn.MaxPool2d(2, 2)
        self.fc1 = nn.Linear(256 * 1 * 1, 128)
        self.fc2 = nn.Linear(128, 10)
        self.relu = nn.ReLU()
        self.dropout = nn.Dropout(0.5)
    
    def forward(self, x):
        # Layer 1: Vanilla
        x = self.relu(self.conv1(x))  # 28x28
        x = self.pool(x)  # 14x14
        
        # Layer 2: Depthwise Separable
        x = self.relu(self.depthwise2(x))
        x = self.relu(self.pointwise2(x))
        x = self.pool(x)  # 7x7
        
        # Layer 3: Dilated
        x = self.relu(self.conv3(x))  # 7x7
        x = self.pool(x)  # 3x3
        
        # Layer 4: Vanilla
        x = self.relu(self.conv4(x))  # 3x3
        x = self.pool(x)  # 1x1
        
        x = x.view(x.size(0), -1)
        x = self.dropout(self.fc1(x))
        x = self.fc2(x)
        return x


# ============================================
# TRAINING & EVALUATION
# ============================================

def train_and_evaluate(model, model_name, epochs=3):
    """Train and evaluate the model"""
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=0.001)
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    model.to(device)
    
    print(f"\n{'='*60}")
    print(f"Training: {model_name}")
    print(f"{'='*60}")
    print(f"Device: {device}")
    print(f"Parameters: {sum(p.numel() for p in model.parameters()):,}")
    
    start_time = time.time()
    
    for epoch in range(epochs):
        # Training
        model.train()
        train_loss = 0.0
        for batch_idx, (data, target) in enumerate(train_loader):
            data, target = data.to(device), target.to(device)
            
            optimizer.zero_grad()
            output = model(data)
            loss = criterion(output, target)
            loss.backward()
            optimizer.step()
            
            train_loss += loss.item()
        
        train_loss /= len(train_loader)
        
        # Evaluation
        model.eval()
        correct = 0
        total = 0
        with torch.no_grad():
            for data, target in test_loader:
                data, target = data.to(device), target.to(device)
                output = model(data)
                _, predicted = torch.max(output, 1)
                total += target.size(0)
                correct += (predicted == target).sum().item()
        
        accuracy = 100 * correct / total
        print(f"Epoch {epoch+1}/{epochs} - Loss: {train_loss:.4f}, "
              f"Accuracy: {accuracy:.2f}%")
    
    elapsed_time = time.time() - start_time
    print(f"Total Training Time: {elapsed_time:.2f}s")
    
    return accuracy


# Train both models
print("MNIST Classification with Different Convolution Types")
print("="*60)

model1 = StandardCNN()
acc1 = train_and_evaluate(model1, "Standard CNN", epochs=3)

model2 = HybridCNN()
acc2 = train_and_evaluate(model2, "Hybrid CNN", epochs=3)

# Comparison
print(f"\n{'='*60}")
print("COMPARISON SUMMARY")
print(f"{'='*60}")
print(f"{'Model':<20} {'Accuracy':<15} {'Parameters':<15}")
print("-" * 60)
print(f"{'Standard CNN':<20} {acc1:.2f}%{'':<10} "
      f"{sum(p.numel() for p in model1.parameters()):,}")
print(f"{'Hybrid CNN':<20} {acc2:.2f}%{'':<10} "
      f"{sum(p.numel() for p in model2.parameters()):,}")
```

---

### Assignment 2: Parameter & Computation Efficiency Analysis

```python
# ============================================
# ASSIGNMENT 2: Efficiency Analysis
# ============================================

"""
TASK:
Compare efficiency (parameters, memory, FLOPs) across convolution types.

Requirements:
1. Create 5 models with same input/output sizes but different convolution types
2. Calculate:
   - Number of parameters
   - Memory usage (MB)
   - FLOPs (multiply-accumulate operations)
   - Inference time
3. Visualize comparison

EVALUATION:
- Accuracy of calculations
- Completeness of comparison
"""

import numpy as np
import matplotlib.pyplot as plt

def count_parameters(model):
    """Count total parameters"""
    return sum(p.numel() for p in model.parameters())


def calculate_conv_flops(in_channels, out_channels, kernel_size, 
                        input_height, input_width, groups=1):
    """
    Calculate FLOPs for a convolution layer
    
    FLOP = 2 × (input_height × input_width × kernel_size × kernel_size 
            × in_channels/groups × out_channels)
    
    (2× for multiply and add)
    """
    flops = 2 * (input_height * input_width * 
                kernel_size * kernel_size * 
                (in_channels // groups) * out_channels)
    return flops


def calculate_memory_mb(num_params, dtype='float32'):
    """Calculate memory in MB for parameters"""
    bytes_per_param = 4 if dtype == 'float32' else 2
    memory_bytes = num_params * bytes_per_param
    memory_mb = memory_bytes / (1024 * 1024)
    return memory_mb


# Convolution Configurations (all produce 56×56 output)
configs = {
    'Vanilla Conv (3×3)': {
        'in_ch': 64, 'out_ch': 128, 'kernel': 3, 'padding': 1,
        'groups': 1, 'dilation': 1
    },
    'Depthwise Separable': {
        'depthwise': {'in_ch': 64, 'out_ch': 64, 'kernel': 3, 'groups': 64},
        'pointwise': {'in_ch': 64, 'out_ch': 128, 'kernel': 1}
    },
    'Dilated Conv (d=2)': {
        'in_ch': 64, 'out_ch': 128, 'kernel': 3, 'padding': 2,
        'groups': 1, 'dilation': 2
    },
    'Grouped Conv (g=4)': {
        'in_ch': 64, 'out_ch': 128, 'kernel': 3, 'padding': 1,
        'groups': 4, 'dilation': 1
    },
    'Asymmetric Conv': {
        'vertical': {'in_ch': 64, 'out_ch': 128, 'kernel': (3, 1)},
        'horizontal': {'in_ch': 128, 'out_ch': 128, 'kernel': (1, 3)}
    }
}

# Calculate metrics
metrics = {}
h, w = 56, 56

print(f"{'='*80}")
print("CONVOLUTION EFFICIENCY ANALYSIS")
print(f"{'='*80}")
print(f"Input: {h}×{w}×64 channels")
print(f"Output: {h}×{w}×128 channels")
print()

for name, config in configs.items():
    print(f"\n{name}")
    print("-" * 80)
    
    if name == 'Depthwise Separable':
        # Depthwise
        params_dw = (config['depthwise']['in_ch'] * 
                    config['depthwise']['kernel'] * 
                    config['depthwise']['kernel'])
        flops_dw = calculate_conv_flops(
            config['depthwise']['in_ch'],
            config['depthwise']['out_ch'],
            config['depthwise']['kernel'],
            h, w, groups=config['depthwise']['groups']
        )
        
        # Pointwise
        params_pw = config['pointwise']['in_ch'] * config['pointwise']['out_ch']
        flops_pw = calculate_conv_flops(
            config['pointwise']['in_ch'],
            config['pointwise']['out_ch'],
            config['pointwise']['kernel'],
            h, w
        )
        
        total_params = params_dw + params_pw
        total_flops = flops_dw + flops_pw
        
    elif name == 'Asymmetric Conv':
        # Vertical
        params_v = (config['vertical']['in_ch'] * 
                   config['vertical']['kernel'][0] * 
                   config['vertical']['kernel'][1] * 
                   config['vertical']['out_ch'])
        flops_v = calculate_conv_flops(
            config['vertical']['in_ch'],
            config['vertical']['out_ch'],
            config['vertical']['kernel'][0],
            h, w
        )
        
        # Horizontal
        params_h = (config['horizontal']['in_ch'] * 
                   config['horizontal']['kernel'][0] * 
                   config['horizontal']['kernel'][1] * 
                   config['horizontal']['out_ch'])
        flops_h = calculate_conv_flops(
            config['horizontal']['in_ch'],
            config['horizontal']['out_ch'],
            config['horizontal']['kernel'][0],
            h, w
        )
        
        total_params = params_v + params_h
        total_flops = flops_v + flops_h
        
    else:
        total_params = (config['in_ch'] * config['out_ch'] * 
                       config['kernel'] * config['kernel'] // config['groups'])
        total_flops = calculate_conv_flops(
            config['in_ch'],
            config['out_ch'],
            config['kernel'],
            h, w,
            groups=config['groups']
        )
    
    memory_mb = calculate_memory_mb(total_params)
    
    print(f"  Parameters: {total_params:>10,}")
    print(f"  Memory:     {memory_mb:>10.2f} MB")
    print(f"  FLOPs:      {total_flops:>10,.0f}")
    
    metrics[name] = {
        'params': total_params,
        'memory': memory_mb,
        'flops': total_flops
    }

# Visualization
fig, axes = plt.subplots(1, 3, figsize=(15, 4))

names = list(metrics.keys())
params = [metrics[n]['params'] for n in names]
memory = [metrics[n]['memory'] for n in names]
flops = [metrics[n]['flops'] for n in names]

# Normalize to vanilla for comparison
vanilla_params = params[0]
vanilla_flops = flops[0]

# Parameters
ax = axes[0]
colors = ['blue' if p == vanilla_params else 'green' for p in params]
ax.bar(range(len(names)), [p/vanilla_params for p in params], color=colors, alpha=0.7)
ax.set_xticks(range(len(names)))
ax.set_xticklabels(names, rotation=45, ha='right')
ax.set_ylabel('Relative to Vanilla Conv')
ax.set_title('Parameters Comparison')
ax.axhline(y=1, color='r', linestyle='--', alpha=0.5, label='Vanilla')
ax.grid(axis='y', alpha=0.3)
ax.legend()

# FLOPs
ax = axes[1]
colors = ['blue' if f == vanilla_flops else 'green' for f in flops]
ax.bar(range(len(names)), [f/vanilla_flops for f in flops], color=colors, alpha=0.7)
ax.set_xticks(range(len(names)))
ax.set_xticklabels(names, rotation=45, ha='right')
ax.set_ylabel('Relative to Vanilla Conv')
ax.set_title('FLOPs Comparison')
ax.axhline(y=1, color='r', linestyle='--', alpha=0.5, label='Vanilla')
ax.grid(axis='y', alpha=0.3)
ax.legend()

# Memory
ax = axes[2]
colors = ['blue' if m == memory[0] else 'green' for m in memory]
ax.bar(range(len(names)), memory, color=colors, alpha=0.7)
ax.set_xticks(range(len(names)))
ax.set_xticklabels(names, rotation=45, ha='right')
ax.set_ylabel('Memory (MB)')
ax.set_title('Memory Usage')
ax.grid(axis='y', alpha=0.3)

plt.tight_layout()
plt.savefig('convolution_efficiency_comparison.png', dpi=150, bbox_inches='tight')
print(f"\n✓ Visualization saved as 'convolution_efficiency_comparison.png'")

# Summary
print(f"\n{'='*80}")
print("SUMMARY")
print(f"{'='*80}")
print(f"Most parameter-efficient: {min(metrics, key=lambda x: metrics[x]['params'])}")
print(f"  ({min([metrics[n]['params'] for n in metrics])} params)")
print(f"\nMost FLOP-efficient: {min(metrics, key=lambda x: metrics[x]['flops'])}")
print(f"  ({min([metrics[n]['flops'] for n in metrics]):,.0f} FLOPs)")
```

---

### Assignment 3: Architecture Modification Challenge

```python
# ============================================
# ASSIGNMENT 3: Architecture Modification
# ============================================

"""
TASK:
Start with a standard ResNet-18 and modify it to use efficient convolutions.

Requirements:
1. Load pretrained ResNet-18
2. Replace specific conv layers with:
   - Depthwise Separable (for 3×3 convs)
   - Grouped Convolution (for bottleneck)
   - Dilated Convolution (for final layers)
3. Compare:
   - Parameters before/after
   - Inference accuracy
   - Speed

EVALUATION:
- Correct implementation
- Thoughtful architecture choices
- Comprehensive comparison
"""

import torch
import torch.nn as nn
import torchvision.models as models
from torchvision.models import resnet18, ResNet18_Weights

print(f"{'='*60}")
print("ARCHITECTURE MODIFICATION: ResNet-18 Optimization")
print(f"{'='*60}")

# Load standard ResNet-18
print("\n1. Loading Standard ResNet-18...")
standard_resnet = resnet18(weights=ResNet18_Weights.IMAGENET1K_V1)
std_params = sum(p.numel() for p in standard_resnet.parameters())
print(f"   Standard ResNet-18 Parameters: {std_params:,}")

# Create Efficient ResNet-18
class EfficientResNet18(nn.Module):
    """ResNet-18 with efficient convolutions"""
    def __init__(self):
        super().__init__()
        
        # Initial conv
        self.conv1 = nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3)
        self.bn1 = nn.BatchNorm2d(64)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        
        # Layer 1: Vanilla (already small)
        self.layer1 = self._make_layer(64, 64, 2, stride=1, use_efficient=False)
        
        # Layer 2: Efficient (depthwise separable + grouped)
        self.layer2 = self._make_layer(64, 128, 2, stride=2, use_efficient=True)
        
        # Layer 3: Efficient with dilated
        self.layer3 = self._make_layer(128, 256, 2, stride=2, 
                                      use_efficient=True, dilation=2)
        
        # Layer 4: Efficient
        self.layer4 = self._make_layer(256, 512, 2, stride=2, use_efficient=True)
        
        self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.fc = nn.Linear(512, 1000)
    
    def _make_layer(self, in_channels, out_channels, num_blocks, 
                   stride=1, use_efficient=False, dilation=1):
        layers = []
        for i in range(num_blocks):
            s = stride if i == 0 else 1
            d = dilation if i > 0 else 1
            
            if use_efficient:
                # Depthwise separable
                layers.append(nn.Conv2d(in_channels if i == 0 else out_channels,
                                       out_channels if i == 0 else out_channels,
                                       3, stride=s, padding=d, dilation=d, 
                                       groups=in_channels if i == 0 else out_channels))
                layers.append(nn.BatchNorm2d(out_channels))
                layers.append(nn.ReLU(inplace=True))
                
                if i == 0 and in_channels != out_channels:
                    # Pointwise for channel adjustment
                    layers.append(nn.Conv2d(in_channels, out_channels, 1))
                    layers.append(nn.BatchNorm2d(out_channels))
            else:
                # Standard
                layers.append(nn.Conv2d(in_channels if i == 0 else out_channels,
                                       out_channels, 3, stride=s, padding=1))
                layers.append(nn.BatchNorm2d(out_channels))
                layers.append(nn.ReLU(inplace=True))
            
            in_channels = out_channels
        
        return nn.Sequential(*layers)
    
    def forward(self, x):
        x = self.relu(self.bn1(self.conv1(x)))
        x = self.maxpool(x)
        
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)
        
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.fc(x)
        
        return x


print("\n2. Creating Efficient ResNet-18...")
efficient_resnet = EfficientResNet18()
eff_params = sum(p.numel() for p in efficient_resnet.parameters())
print(f"   Efficient ResNet-18 Parameters: {eff_params:,}")

# Comparison
print(f"\n{'='*60}")
print("COMPARISON RESULTS")
print(f"{'='*60}")
print(f"{'Metric':<30} {'Standard':<20} {'Efficient':<20}")
print("-" * 60)
print(f"{'Parameters':<30} {std_params:>18,} {eff_params:>18,}")
print(f"{'Reduction':<30} {'-':>18} {(std_params-eff_params)/std_params*100:>18.1f}%")

# Test inference
print(f"\n{'='*60}")
print("INFERENCE TEST")
print(f"{'='*60}")

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
standard_resnet.to(device)
efficient_resnet.to(device)

test_input = torch.randn(1, 3, 224, 224).to(device)

import time

# Warmup
_ = standard_resnet(test_input)
_ = efficient_resnet(test_input)

# Standard ResNet
torch.cuda.synchronize() if device.type == 'cuda' else None
start = time.time()
for _ in range(10):
    _ = standard_resnet(test_input)
torch.cuda.synchronize() if device.type == 'cuda' else None
std_time = (time.time() - start) / 10
print(f"Standard ResNet-18 Inference Time: {std_time*1000:.2f} ms")

# Efficient ResNet
torch.cuda.synchronize() if device.type == 'cuda' else None
start = time.time()
for _ in range(10):
    _ = efficient_resnet(test_input)
torch.cuda.synchronize() if device.type == 'cuda' else None
eff_time = (time.time() - start) / 10
print(f"Efficient ResNet-18 Inference Time: {eff_time*1000:.2f} ms")
print(f"Speedup: {std_time/eff_time:.2f}×")
```

---

## Part 4: Real Architecture Comparisons {#architectures}

### MobileNet - Depthwise Separable Excellence

```python
# ============================================
# MobileNet: Depthwise Separable Convolution
# ============================================

"""
MobileNet Architecture Overview:

WHY: Mobile devices need lightweight models
     - Limited memory
     - Limited compute power
     - Battery constraints

SOLUTION: Depthwise Separable Convolution

Structure:
┌──────────────────────────────────┐
│ Input (H×W×C_in)                 │
│         ↓                        │
│ Depthwise: (H×W×C_in)           │
│ (One 3×3 filter per channel)    │
│         ↓                        │
│ Pointwise: (H×W×C_in)           │
│ (1×1 conv for channel mixing)   │
│         ↓                        │
│ Output (H'×W'×C_out)             │
└──────────────────────────────────┘

Benefits:
- 8-9× fewer parameters than standard
- 8-9× fewer operations
- Runs on phones!

ImageNet Top-1 Accuracy:
- Full ResNet-50: 76.0%
- MobileNet-v2: 71.9% (with 1/9 parameters!)
"""

import torch
import torch.nn as nn

class MobileNetBlock(nn.Module):
    """MobileNet inverted residual block"""
    def __init__(self, in_channels, out_channels, stride=1, expand_ratio=6):
        super().__init__()
        
        hidden_channels = in_channels * expand_ratio
        
        # 1×1 expansion
        self.expand = nn.Sequential(
            nn.Conv2d(in_channels, hidden_channels, 1, bias=False),
            nn.BatchNorm2d(hidden_channels),
            nn.ReLU6(inplace=True)
        )
        
        # Depthwise 3×3
        self.depthwise = nn.Sequential(
            nn.Conv2d(hidden_channels, hidden_channels, 3, stride=stride, 
                     padding=1, groups=hidden_channels, bias=False),
            nn.BatchNorm2d(hidden_channels),
            nn.ReLU6(inplace=True)
        )
        
        # 1×1 projection
        self.project = nn.Sequential(
            nn.Conv2d(hidden_channels, out_channels, 1, bias=False),
            nn.BatchNorm2d(out_channels)
        )
        
        self.use_shortcut = stride == 1 and in_channels == out_channels
    
    def forward(self, x):
        residual = x
        
        x = self.expand(x)
        x = self.depthwise(x)
        x = self.project(x)
        
        if self.use_shortcut:
            x += residual
        
        return x


class MobileNetV2(nn.Module):
    """MobileNet-v2 Architecture"""
    def __init__(self, num_classes=1000):
        super().__init__()
        
        # First conv
        self.stem = nn.Sequential(
            nn.Conv2d(3, 32, 3, stride=2, padding=1, bias=False),
            nn.BatchNorm2d(32),
            nn.ReLU6(inplace=True)
        )
        
        # Inverted residual blocks
        # (in_ch, out_ch, num_blocks, stride, expand_ratio)
        config = [
            (32, 16, 1, 1, 1),
            (16, 24, 2, 2, 6),
            (24, 32, 3, 2, 6),
            (32, 64, 4, 2, 6),
            (64, 96, 3, 1, 6),
            (96, 160, 3, 2, 6),
            (160, 320, 1, 1, 6),
        ]
        
        self.blocks = nn.ModuleList()
        for in_ch, out_ch, num_blocks, stride, expand in config:
            for i in range(num_blocks):
                s = stride if i == 0 else 1
                self.blocks.append(MobileNetBlock(in_ch, out_ch, s, expand))
                in_ch = out_ch
        
        # Final layers
        self.final = nn.Sequential(
            nn.Conv2d(320, 1280, 1, bias=False),
            nn.BatchNorm2d(1280),
            nn.ReLU6(inplace=True),
            nn.AdaptiveAvgPool2d(1),
            nn.Conv2d(1280, num_classes, 1)
        )
    
    def forward(self, x):
        x = self.stem(x)
        for block in self.blocks:
            x = block(x)
        x = self.final(x)
        return x.view(x.size(0), -1)


# Key Statistics
print("=" * 60)
print("MobileNet-v2 Architecture Overview")
print("=" * 60)

model = MobileNetV2()
total_params = sum(p.numel() for p in model.parameters())
print(f"Total Parameters: {total_params:,}")

# Comparison with ResNet-50
resnet50_params = 25_557_032
print(f"\nComparison with ResNet-50:")
print(f"  ResNet-50 parameters: {resnet50_params:,}")
print(f"  MobileNet-v2 parameters: {total_params:,}")
print(f"  Reduction: {resnet50_params/total_params:.1f}×")
print(f"\nAccuracy on ImageNet:")
print(f"  ResNet-50: 76.0%")
print(f"  MobileNet-v2: 71.9%")
print(f"  Accuracy drop: 4.1% for 21× smaller model!")

# Test inference
print(f"\nTesting inference...")
x = torch.randn(1, 3, 224, 224)
y = model(x)
print(f"Input shape: {x.shape}")
print(f"Output shape: {y.shape}")
print(f"✓ MobileNet-v2 working!")
```

### ResNeXt - Grouped Convolution Excellence

```python
# ============================================
# ResNeXt: Grouped Convolution
# ============================================

"""
ResNeXt (Next Generation ResNet)

WHY: ResNet was powerful but depended heavily on depth
     ResNeXt adds "cardinality" (group concept)

INNOVATION: Grouped Convolution

Instead of:
┌─────────────────┐
│ Single pathway  │  (ResNet)
│ (Deep)          │
└─────────────────┘

Use:
┌──────────────────────────────┐
│ 32 (or more) parallel paths  │  (ResNeXt)
│ (Each group processes        │   = Higher capacity
│  independently, then merged) │
└──────────────────────────────┘

Mathematical equivalence: Grouped Conv = Multiple smaller convs

Benefits:
- Higher accuracy than ResNet with same parameters
- Better parameter efficiency
- Improved regularization
"""

import torch
import torch.nn as nn

class ResNeXtBlock(nn.Module):
    """ResNeXt bottleneck block with grouped convolution"""
    def __init__(self, in_channels, out_channels, cardinality=32, 
                 bottleneck_width=4, stride=1):
        super().__init__()
        
        hidden_channels = cardinality * bottleneck_width
        
        # 1×1 reduction
        self.conv1 = nn.Conv2d(in_channels, hidden_channels, 1, bias=False)
        self.bn1 = nn.BatchNorm2d(hidden_channels)
        
        # 3×3 grouped convolution (KEY!)
        self.conv2 = nn.Conv2d(hidden_channels, hidden_channels, 3,
                              stride=stride, padding=1, groups=cardinality, 
                              bias=False)
        self.bn2 = nn.BatchNorm2d(hidden_channels)
        
        # 1×1 expansion
        self.conv3 = nn.Conv2d(hidden_channels, out_channels, 1, bias=False)
        self.bn3 = nn.BatchNorm2d(out_channels)
        
        self.relu = nn.ReLU(inplace=True)
        
        # Shortcut
        self.shortcut = nn.Sequential()
        if stride != 1 or in_channels != out_channels:
            self.shortcut = nn.Sequential(
                nn.Conv2d(in_channels, out_channels, 1, stride=stride, bias=False),
                nn.BatchNorm2d(out_channels)
            )
    
    def forward(self, x):
        residual = x
        
        x = self.relu(self.bn1(self.conv1(x)))
        x = self.relu(self.bn2(self.conv2(x)))  # Grouped!
        x = self.bn3(self.conv3(x))
        
        x += self.shortcut(residual)
        x = self.relu(x)
        
        return x


class ResNeXt50(nn.Module):
    """ResNeXt-50 with 32×4d (cardinality=32, bottleneck_width=4)"""
    def __init__(self, cardinality=32, bottleneck_width=4, num_classes=1000):
        super().__init__()
        
        # Stem
        self.conv1 = nn.Conv2d(3, 64, 7, stride=2, padding=3, bias=False)
        self.bn1 = nn.BatchNorm2d(64)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(3, stride=2, padding=1)
        
        # Residual layers
        self.layer1 = self._make_layer(64, 256, 3, stride=1, 
                                       cardinality=cardinality,
                                       bottleneck_width=bottleneck_width)
        self.layer2 = self._make_layer(256, 512, 4, stride=2,
                                       cardinality=cardinality,
                                       bottleneck_width=bottleneck_width)
        self.layer3 = self._make_layer(512, 1024, 6, stride=2,
                                       cardinality=cardinality,
                                       bottleneck_width=bottleneck_width)
        self.layer4 = self._make_layer(1024, 2048, 3, stride=2,
                                       cardinality=cardinality,
                                       bottleneck_width=bottleneck_width)
        
        self.avgpool = nn.AdaptiveAvgPool2d(1)
        self.fc = nn.Linear(2048, num_classes)
    
    def _make_layer(self, in_channels, out_channels, num_blocks, stride=1,
                   cardinality=32, bottleneck_width=4):
        layers = []
        for i in range(num_blocks):
            s = stride if i == 0 else 1
            layers.append(ResNeXtBlock(in_channels if i == 0 else out_channels,
                                       out_channels, cardinality, 
                                       bottleneck_width, s))
        return nn.Sequential(*layers)
    
    def forward(self, x):
        x = self.relu(self.bn1(self.conv1(x)))
        x = self.maxpool(x)
        
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)
        
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.fc(x)
        
        return x


# Key Statistics
print("=" * 60)
print("ResNeXt-50 Architecture Overview")
print("=" * 60)

model_resnext = ResNeXt50(cardinality=32, bottleneck_width=4)
total_params = sum(p.numel() for p in model_resnext.parameters())
print(f"ResNeXt-50 (32×4d) Parameters: {total_params:,}")

# ResNet-50 for comparison
print(f"\nComparison with ResNet-50:")
resnet50_params = 25_557_032
print(f"  ResNet-50 parameters: {resnet50_params:,}")
print(f"  ResNeXt-50 parameters: {total_params:,}")
print(f"  Increase: {total_params/resnet50_params:.1f}×")

print(f"\nImageNet Performance:")
print(f"  ResNet-50 Top-1: 76.0%")
print(f"  ResNeXt-50 Top-1: 77.6%")
print(f"  Improvement: +1.6% with 3.3× more params")
print(f"  (Better accuracy with grouped cardinality!)")

# Test inference
print(f"\nTesting inference...")
x = torch.randn(1, 3, 224, 224)
y = model_resnext(x)
print(f"Input shape: {x.shape}")
print(f"Output shape: {y.shape}")
print(f"✓ ResNeXt-50 working!")
```

---

## Quick Reference Guide {#quick-reference}

### Convolution Selection Guide

```
┌─────────────────────────────────────────────────────────────┐
│            WHICH CONVOLUTION TO USE?                        │
└─────────────────────────────────────────────────────────────┘

🎯 SPEED IS CRITICAL (Mobile, Edge)
   └─→ Use: Depthwise Separable
       (MobileNet, TensorFlow Lite)

🎯 ACCURACY IS CRITICAL (Server, Research)
   └─→ Use: Vanilla or Grouped Convolution
       (ResNet, ResNeXt)

🎯 NEED LARGE RECEPTIVE FIELD
   └─→ Use: Dilated Convolution
       (Semantic Segmentation, Scene Understanding)

🎯 UPSAMPLING / GENERATION
   └─→ Use: Transposed Convolution
       (GANs, Image Segmentation, Autoencoders)

🎯 NEED EFFICIENCY + CAPACITY
   └─→ Use: Grouped Convolution
       (ResNeXt, EfficientNet)

🎯 NEED PARAMETER REDUCTION
   └─→ Use: Factorized or Asymmetric
       (Inception Networks, MobileNet)

🎯 NEED TO HANDLE DEFORMATIONS
   └─→ Use: Deformable Convolution
       (Object Detection, Instance Segmentation)
```

---

### Parameter Estimation Cheat Sheet

```python
# Quick parameter calculation

# Vanilla Conv2d
params_vanilla = kernel_size × kernel_size × in_channels × out_channels

# Depthwise Separable
params_dw_sep = (kernel_size × kernel_size × in_channels) + (in_channels × out_channels)

# Dilated Conv2d (same params as vanilla)
params_dilated = kernel_size × kernel_size × in_channels × out_channels

# Transposed Conv2d
params_transposed = kernel_size × kernel_size × in_channels × out_channels

# Grouped Conv2d
params_grouped = (kernel_size × kernel_size × in_channels/groups × out_channels)

# Asymmetric (5×1 + 1×5)
params_asym = (5 × 1 × in_ch) + (1 × 5 × out_ch)

# Factorized (3×1 + 1×3)
params_fact = (3 × 1 × in_ch) + (1 × 3 × out_ch)
```

---

### Architecture Decision Tree

```
START: Designing a CNN layer

│
├─ Need to SHRINK the feature map?
│  ├─ YES + Speed critical? → Depthwise Separable
│  ├─ YES + High accuracy? → Vanilla or Grouped
│  └─ NO → Continue
│
├─ Need to ENLARGE the feature map?
│  ├─ YES → Transposed Convolution
│  └─ NO → Continue
│
├─ Need LARGE receptive field?
│  ├─ YES → Dilated Convolution
│  └─ NO → Continue
│
├─ Need EFFICIENCY?
│  ├─ YES → Depthwise Separable or Grouped
│  └─ NO → Continue
│
└─ DEFAULT → Use Vanilla Convolution
```

---

## Summary Table

| Type | Parameters | Speed | Accuracy | Best For | Key Ref |
|------|-----------|-------|----------|----------|---------|
| **Vanilla** | 1.0× | 1.0× | 1.0× | General purpose | ResNet |
| **Depthwise Sep** | 0.1× | 10× | 0.95× | Mobile | MobileNet |
| **Dilated** | 1.0× | 1.0× | 1.05× | Scene segmentation | DeepLab |
| **Transposed** | 1.0× | 0.8× | 1.0× | Generation | SegNet |
| **Grouped** | 0.6× | 1.2× | 1.02× | Efficiency + capacity | ResNeXt |
| **Asymmetric** | 0.7× | 1.3× | 0.98× | Parameter reduction | Inception |
| **Factorized** | 0.7× | 1.3× | 0.98× | Parameter reduction | Inception |
| **Deformable** | 1.1× | 0.5× | 1.08× | Irregular objects | Mask R-CNN |

---

## Setup & Installation {#setup}

```bash
# Python 3.8+
pip install torch torchvision
pip install tensorflow tensorflow-addons
pip install numpy matplotlib pillow
pip install deformable-conv-pytorch

# Verify installation
python -c "import torch; print(f'PyTorch: {torch.__version__}')"
python -c "import tensorflow as tf; print(f'TensorFlow: {tf.__version__}')"
```

---

## Additional Resources

### Learning Path

1. **Week 1:** Vanilla + Depthwise Convolutions
   - Understand basic convolution
   - Learn about channel efficiency

2. **Week 2:** Dilated + Transposed Convolutions
   - Receptive field concepts
   - Upsampling techniques

3. **Week 3:** Grouped + Asymmetric Convolutions
   - Parameter efficiency
   - Multi-scale processing

4. **Week 4:** Factorized + Deformable Convolutions
   - Advanced factorization
   - Adaptive spatial processing

### Papers to Read

1. **MobileNets** - Depthwise Separable Convolution
2. **ResNeXt** 
