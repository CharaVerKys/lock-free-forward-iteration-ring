# lock-free-forward-iteration-ring
specific task thread safe ring


a operations with this ring aren't truly thread-safe
i use buffer  `m_size*n`, if your read operations too long set bigger N
read operation assumed to perform `one run == full iteration`
