#!/usr/bin/env python3
import torch
import torch.nn as nn
import torch.nn.functional as F
from typing import List
import math
from torch.utils.tensorboard import SummaryWriter


class char_tokenizer:
    """
    a very simple char-based tokenizer. the tokenizer turns a string into a list of integers.
    """

    def __init__(self, corpus: List[str]):
        self.corpus = corpus
        # calculate the vocab size and create a dictionary that maps each character to a unique integer
        self.n_vocab = len(corpus)
        self.token2idx = {t: i for i, t in enumerate(corpus)}
        self.idx2token = {i: t for i, t in enumerate(corpus)}

    def encode(self, string: str):
        # convert a string into a list of integers and return, using the dictionary you created above
        return [self.token2idx[t] for t in string]

    def decode(self, codes: List[int]):
        # convert a list of integers into a string and return, using the dictionary you created above
        return "".join([self.idx2token[c] for c in codes])


class Head(nn.Module):
    """single head of self-attention"""

    def __init__(self, n_embd, head_size):
        super().__init__()
        # create three linear layers, Key, Query, and Value, each of which maps from n_embd to head_size
        self.Key = nn.Linear(n_embd, head_size, bias=False)
        self.Query = nn.Linear(n_embd, head_size, bias=False)
        self.Value = nn.Linear(n_embd, head_size, bias=False)
        self.head_size = head_size
        self.register_buffer("tril", torch.tril(torch.ones(1000, 1000)))

    def forward(self, inputs):
        # implement the forward function of the head
        # the input is a tensor of shape (batch, time, n_embd)
        # the output should be a tensor of shape (batch, time, head_size)
        # you may use the tril buffer defined above to mask out the upper triangular part of the affinity matrix
        query = self.Query(inputs)
        key = self.Key(inputs)
        value = self.Value(inputs)
        scale = self.head_size ** -0.5
        logits = torch.bmm(query, key.transpose(1, 2)) * scale
        logits.masked_fill_(self.tril[:inputs.size(1), :inputs.size(1)] == 0, float("-inf"))
        weights = F.softmax(logits, dim=-1)
        out = torch.bmm(weights, value)
        return out


class MultiHeadAttention(nn.Module):
    def __init__(self, n_heads, n_embd):
        super().__init__()
        # implement heads and projection
        head_size = n_embd // n_heads
        self.heads = nn.ModuleList([Head(n_embd, head_size) for _ in range(n_heads)])
        self.projection = nn.Linear(n_embd, n_embd)

    def forward(self, inputs):
        # implement the forward function of the multi-head attention
        out = torch.cat([head(inputs) for head in self.heads], dim=-1)
        return self.projection(out)


class FeedForward(nn.Module):
    def __init__(self, n_embd):
        super().__init__()
        # implement the feed-forward network
        self.net = nn.Sequential(
            nn.Linear(n_embd, 4 * n_embd),
            nn.ReLU(),
            nn.Linear(4 * n_embd, n_embd),
        )

    def forward(self, inputs):
        return self.net(inputs)


class Block(nn.Module):
    def __init__(self, n_embd, n_heads):
        super().__init__()
        # implement the block of transformer using the MultiHeadAttention and FeedForward modules,
        # along with the layer normalization layers
        self.attention = nn.LayerNorm(n_embd)
        self.feedforward = nn.LayerNorm(n_embd)
        self.multihead = MultiHeadAttention(n_heads, n_embd)
        self.ffn = FeedForward(n_embd)

    def forward(self, inputs):
        # implement the forward function of the block, you may refer to the docs of this experiment
        x = inputs + self.attention(self.multihead(inputs))
        x = x + self.feedforward(self.ffn(x))
        return x


class PositionalEncoding(nn.Module):
    def __init__(self, d_model, max_len=1000):
        super().__init__()

        # 创建位置编码(类似掩码实现的方式),采用正弦的方式编码位置信息
        pe = torch.zeros(max_len, d_model)
        position = torch.arange(0, max_len, dtype=torch.float).unsqueeze(1)
        div_term = torch.exp(torch.arange(0, d_model, 2).float() * (-math.log(10000.0) / d_model))
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term)
        pe = pe.unsqueeze(0)
        self.register_buffer('pe', pe)

    def forward(self, x):
        x = x + self.pe[:, :x.size(1)]
        return x


class Transformer(nn.Module):
    def __init__(self):
        super().__init__()

        # create the embedding table, the stack of blocks, the layer normalization layer,
        # and the linear layers.

        # 文本嵌入层
        self.embedding = nn.Embedding(num_embeddings=n_vocab, embedding_dim=n_embd)
        # 位置编码层
        self.positional_encoding = PositionalEncoding(d_model=n_embd)
        # 若干层Block
        self.blocks = nn.ModuleList([Block(n_embd, n_heads) for _ in range(n_layers)])
        # 创建层归一化
        self.norm = nn.LayerNorm(n_embd)
        # 线性层
        self.fc1 = nn.Linear(n_embd, n_vocab)

    def forward(self, inputs, labels=None):
        # implement the forward function of the transformer

        # inputs:(batch, context)
        batch, time = inputs.shape
        # embedding:(batch, context, embedding)
        embedding = self.embedding(inputs)
        # add positional encoding
        embedding = self.positional_encoding(embedding)
        # attens:(batch, context, embedding)
        attens = embedding
        # apply the stack of blocks
        for block in self.blocks:
            attens = block(attens)
        # attens:(batch * context, embedding)
        attens = attens.view(batch * time, -1)
        # logits:(batch * context, vocab_size)
        logits = self.fc1(attens)
        # logits:(batch, context, vocab_size)
        logits = logits.view(batch, time, -1)

        # compute the loss
        if labels is None:
            loss = None
        else:
            batch, time, channel = logits.shape
            logits = logits.view(batch*time, channel)
            labels = labels.view(-1)
            loss = F.cross_entropy(logits, labels)
        return logits, loss


    def generate(self, inputs, max_new_tokens):
            # generate new tokens from the transformer, using the inputs as the context,
            # and return the generated tokens with length of max_new_tokens

            for _ in range(max_new_tokens):
                # generates new tokens by iteratively sampling from the model's predicted probability distribution,
                # concatenating the sampled tokens to the input sequence, and returning the updated sequence.

                # 文本编码
                embedding = self.embedding(inputs)
                # 位置编码
                embedding = self.positional_encoding(embedding)
                attens = embedding  
                # 若干层Block
                for block in self.blocks:
                    attens = block(attens)
                attens = attens.view(-1, n_embd)

                logits = self.fc1(attens)
                # 使用softmax层获得最大可能预测的输出
                probabilities = F.softmax(logits, dim=1)
                samples = torch.multinomial(probabilities, num_samples=1)
                # 将预测的最后一个词扩充输入
                inputs = torch.cat([inputs, samples[-1].unsqueeze(0)], dim=1)
                
            return inputs


def get_batch(split):
    data = train_data if split == "train" else val_data
    ix = torch.randint(len(data) - block_size, (batch_size,))
    x = torch.stack([data[i : i + block_size] for i in ix])
    y = torch.stack([data[i + 1 : i + block_size + 1] for i in ix])
    x, y = x.to(device), y.to(device)
    return x, y


@torch.no_grad()
def estimate_loss(model):
    out = {}
    model.eval()
    for split in ["train", "val"]:
        losses = torch.zeros(eval_iters)
        for k in range(eval_iters):
            x, y = get_batch(split)
            logits, loss = model(x, y)
            losses[k] = loss.item()
        out[split] = losses.mean()
    return out


def generate(model):
    text = "First Citizen: If I must not, I need not be barren of accusations;"
    context = torch.tensor(encode(text), dtype=torch.long).unsqueeze(0).to(device)
    print(decode(model.generate(context, max_new_tokens=500)[0].tolist()))


def train(model):
    optimizer = torch.optim.AdamW(model.parameters(), lr=learning_rate)

    # 使用tensorboard保存日志
    writer = SummaryWriter(log_dir="../logs")  # create a summary writer object

    for iter in range(max_iters):

        if iter % eval_interval == 0:
            losses = estimate_loss(model)
            print(
                f"step {iter}: train loss {losses['train']:.4f}, val loss {losses['val']:.4f}"
            )

            # 保存loss信息到tensorboard
            writer.add_scalar("Loss/train", losses["train"], iter)
            writer.add_scalar("Loss/val", losses["val"], iter)

        inputs, labels = get_batch("train")

        logits, loss = model(inputs, labels)
        optimizer.zero_grad(set_to_none=True)
        loss.backward()
        optimizer.step()
        
        writer.add_scalar("Loss/train_batch", loss, iter)

    writer.close()  


# define the hyperparameters
batch_size = 16
block_size = 256
max_iters = 100000 # set the number of training iterations as you like
eval_interval = 50
learning_rate = 1e-3
device = "cuda" if torch.cuda.is_available() else "cpu"
eval_iters = 200
n_embd = 64
n_heads = 8
n_layers = 6

# read the dataset
with open("../data/input.txt", "r", encoding="utf-8") as f:
    text = f.read()
chars = sorted(list(set(text)))

# initialize the vocabulary
tokenizer = char_tokenizer(chars)
encode = tokenizer.encode
decode = tokenizer.decode
n_vocab = tokenizer.n_vocab

# separate the dataset into train and validation
train_data = torch.tensor(encode(text[: -len(text) // 10]), dtype=torch.long)
val_data = torch.tensor(encode(text[-len(text) // 10 :]), dtype=torch.long)

# define the model
model = Transformer().to(device)
train(model)
generate(model)
