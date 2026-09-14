# fixture

## Objects

### Every scalar type

**0x2000**
One entry per supported scalar type

| Name              | Type    | default |
| ----------------- | ------- | ------- |
| Number of entries | uint8   | 10      |
| A boolean         | bool    | 1       |
| An int8           | int8    | -8      |
| An int16          | int16   | -16     |
| An int32          | int32   | -32     |
| An int64          | int64   | -64     |
| A uint16          | uint16  | 16      |
| A uint32          | uint32  | 32      |
| A uint64          | uint64  | 64      |
| A float32         | float32 | 1.5     |
| A float64         | float64 | 2.5     |

### A string

**0x2001**

| Name     | Type   | default |
| -------- | ------ | ------- |
| A string | string |         |

### A domain

**0x2002**

| Name     | Type   | default |
| -------- | ------ | ------- |
| A domain | domain | 0       |

### A limited value

**0x2003**

| Name            | Type   | default |
| --------------- | ------ | ------- |
| A limited value | uint16 | 50      |

### An array

**0x2004**

| Name              | Type   | default |
| ----------------- | ------ | ------- |
| Number of entries | uint8  | 3       |
| First element     | uint32 | 1       |
| Second element    | uint32 | 2       |
| Third element     | uint32 | 3       |

### A remote value

**0x2005**

| Name           | Type   | default |
| -------------- | ------ | ------- |
| A remote value | uint32 | 0       |

### A remote attribute

**0x2006**

| Name               | Type    | default |
| ------------------ | ------- | ------- |
| A remote attribute | float32 | 0       |

### An enumerated value

**0x2007**

| Name                | Type  | default |
| ------------------- | ----- | ------- |
| An enumerated value | uint8 | 0       |
