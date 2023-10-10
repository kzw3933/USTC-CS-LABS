import React, { useState, useEffect, useContext } from 'react';
import { Button, Container,  Table, Modal, Form, Col } from 'react-bootstrap';

import { HOST, PORT, DOMAIN } from '../config';
import { teacherContext } from './App';

const PostForm = (props) => {
  const { postSN, setPostSN} = props;

  const handlePostSNChange = (event) => {
    setPostSN(event.target.value);
  };

  const handleButtonClick = async () => {
    try {
      const res = await fetch(`${HOST}:${PORT}/api/paper/paper/${postSN}`, { method: 'GET' });
      const result = await res.json();
      if (res.ok) {
        const form = document.forms.postForm;
        form.sn.value = result.data.sn;
        form.title.value = result.data.title;
        form.publicSource.value = result.data.publicSource;
        form.publicationYear.value = result.data.publicationYear;
        form.publicType.value = result.data.publicType;
        form.publicRank.value = result.data.publicRank;
      } else {
        alert(result.message);
      }
    } catch (err) {
      alert(err.message);
    }
  };

  return (
    <Form id="postForm">
        <Form.Group controlId="pid1">
            <Form.Label>待载入数据的论文号</Form.Label>
            <Form.Control type="text" placeholder="请输入待载入论文的论文号" value={postSN} onChange={handlePostSNChange} />
        </Form.Group>
        <Form.Group id="button">
            <Button variant="primary" className="ml-2" onClick={handleButtonClick}>载入</Button>
        </Form.Group>
        <Form.Group controlId="sn">
            <Form.Label>论文号</Form.Label>
            <Form.Control type="text" placeholder="请输入论文号"/>
        </Form.Group>
        <Form.Group controlId="title">
            <Form.Label>论文名称</Form.Label>
            <Form.Control type="text" placeholder="请输入论文名称"/>
        </Form.Group>
        <Form.Group controlId="publicSource">
            <Form.Label>发表源</Form.Label>
            <Form.Control type="text" placeholder="请输入发表源"/>
        </Form.Group>
        <Form.Group controlId="publicationYear">
            <Form.Label>发表年份</Form.Label>
            <Form.Control type="text" placeholder="请输入发表年份"/>
        </Form.Group>
        <Form.Group controlId="publicType">
            <Form.Label>类型</Form.Label>
            <Form.Control as="select">
              <option value="">无</option>
              <option value="1">full paper</option>
              <option value="2">short paper</option>
              <option value="3">poster paper</option>
              <option value="4">demo paper</option>
            </Form.Control>
        </Form.Group>
        <Form.Group controlId="publicRank">
            <Form.Label>级别</Form.Label>
            <Form.Control as="select">
              <option value="">无</option>
              <option value="1">CCF-A</option>
              <option value="2">CCF-B</option>
              <option value="3">CCF-C</option>
              <option value="4">中文CCF-A</option>
              <option value="5">中文CCF-B</option>
              <option value="6">无级别</option>
            </Form.Control>
        </Form.Group>
        <Form.Group controlId="tid">
            <Form.Label>教师号</Form.Label>
            <Form.Control type="text" placeholder="请输入教师号"/>
        </Form.Group>
        <Form.Group controlId="paperRank">
            <Form.Label>排名</Form.Label>
            <Form.Control type="text" placeholder="请输入排名"/>
        </Form.Group>
        <Form.Group controlId="isCorrespondingAuthor">
            <Form.Label>是否通讯作者</Form.Label>
            <Form.Control as="select">
              <option value="">无</option>
              <option value="1">是</option>
              <option value="0">否</option>
            </Form.Control>
        </Form.Group>
    </Form>
  );
}

const PostButton = (props) => {
  const {loadPapers} = props;
  const [show, setShow] = useState(false);
  const showModal = () => setShow(true);
  const closeModal = () => setShow(false);

  const [postSN, setPostSN] = useState("");
  const [postPapers, setPostPapers] = useState([]);
  const post = async () => {
      try {
          const postSNs = []
          const postTids = []
          const paperRanks = []

          const CorrespondingAuthors = []
          
          console.log(postPapers);
          postPapers.forEach(element => {
            if(! postSNs.includes(element.sn)) {
              postSNs.push(element.sn);
              if(postSNs.length > 1) {
                throw new Error("不支持同时新增涉及多篇论文的数据");
              }
            }
            if(! postTids.includes(element.tid)) {
              postTids.push(element.sn);
            }
            if(paperRanks.includes(element.paperRank)) {
              throw new Error("新增数据中排名存在重复");
            } 
            paperRanks.push(element.paperRank);
            
            if(parseInt(element.isCorrespondingAuthor) && !CorrespondingAuthors.includes(element.tid)) {
              CorrespondingAuthors.push(element.tid)
              if(CorrespondingAuthors.length > 1) {
                throw new Error("不允许一篇论文同时有多个通讯作者");
              }
            }
          })
          
          const body = {postPapers, postTids};

          const res = await fetch(`${HOST}:${PORT}/api/paper/admin`, {
              method: 'POST',
              headers: { 'Content-Type': 'application/json' },
              body: JSON.stringify(body)
          }) ;
          const result = await res.json();
          if(res.ok) {
              alert(result.message);
              closeModal();
              loadPapers();
              setPostPapers([])
          } else {
              alert(result.message);
              closeModal();
              setPostPapers([])
          }
      } catch(err) {
          alert(err.message);
          closeModal();
          setPostPapers([]);
      } 
  }

  const handleSave = async () => {
      const form = document.forms.postForm;
      const sn = form.sn.value;
      const tid = form.tid.value;
      const title = form.title.value;
      const publicSource = form.publicSource.value;
      const publicationYear = form.publicationYear.value;
      const publicType = form.publicType.value;
      const publicRank = form.publicRank.value;
      const paperRank = form.paperRank.value;
      const isCorrespondingAuthor = form.isCorrespondingAuthor.value;
      const body = { 
          sn, title, publicSource, publicationYear, publicType, publicRank, paperRank, isCorrespondingAuthor, tid
      };
      setPostPapers([...postPapers, body]);
  }
  return (
      <div>
          <Button variant="outline-success" size="sm" onClick={() => showModal()}>
              新建论文
          </Button>
          <Modal show={show} onHide={() => closeModal()}>
              <Modal.Header closeButton>
                  <Modal.Title>新建论文</Modal.Title>
              </Modal.Header>
              <Modal.Body>
                  <PostForm setPostSN={setPostSN} postSN={postSN}/>
              </Modal.Body>
              <Modal.Footer>
                  <Button variant="secondary" onClick={() => closeModal() }>
                      关闭
                  </Button>
                  <Button variant="secondary" onClick={() => handleSave()}>
                      保存
                  </Button>
                  <Button variant="primary" onClick={() => post()}>
                      提交
                  </Button>
              </Modal.Footer>
          </Modal>
      </div>
  );
}

const ModifyForm = (props) => {
    const { modifySN, setModifySN, modifyTid, setModifyTid } = props;
    
    const handleModifySNChange = (event) => {
      setModifySN(event.target.value);
    };

    const handleModifyTidChange = (event) => {
        setModifyTid(event.target.value);
      };

    const handleButtonClick = async () => {
      try {
        const res = await fetch(`${HOST}:${PORT}/api/paper/${modifyTid}/${modifySN}`, { method: 'GET' });
        const result = await res.json();
        if (res.ok) {
          const form = document.forms.modifyForm;
          form.title.value = result.data.title;
          form.publicSource.value = result.data.publicSource;
          form.publicationYear.value = result.data.publicationYear;
          form.publicType.value = result.data.publicType;
          form.publicRank.value = result.data.publicRank;
          form.paperRank.value = result.data.paperRank;
          form.isCorrespondingAuthor.value = result.data.isCorrespondingAuthor;
        } else {
          alert(result.message);
        }
      } catch (err) {
        alert(err.message);
      }
    };

    return (
      <Form id="modifyForm">
        <Form.Group controlId="mid1">
            <Form.Label>论文号</Form.Label>
            <Form.Control type="text" placeholder="请输入待修改论文的论文号" value={modifySN} onChange={handleModifySNChange} />
        </Form.Group>
        <Form.Group controlId="mid2">
            <Form.Label>教师号</Form.Label>
            <Form.Control type="text" placeholder="请输入待修改论文作者的教师号" value={modifyTid} onChange={handleModifyTidChange} />
        </Form.Group>
        <Form.Group id="button">
            <Button variant="primary" className="ml-2" onClick={handleButtonClick}>载入</Button>
        </Form.Group>

          <Form.Group controlId="title">
              <Form.Label>论文名称</Form.Label>
              <Form.Control type="text" placeholder="请输入论文名称"/>
          </Form.Group>
          <Form.Group controlId="publicSource">
              <Form.Label>发表源</Form.Label>
              <Form.Control type="text" placeholder="请输入发表源"/>
          </Form.Group>
          <Form.Group controlId="publicationYear">
              <Form.Label>发表年份</Form.Label>
              <Form.Control type="text" placeholder="请输入发表年份"/>
          </Form.Group>
          <Form.Group controlId="publicType">
              <Form.Label>类型</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">full paper</option>
                <option value="2">short paper</option>
                <option value="3">poster paper</option>
                <option value="4">demo paper</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="publicRank">
              <Form.Label>级别</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">CCF-A</option>
                <option value="2">CCF-B</option>
                <option value="3">CCF-C</option>
                <option value="4">中文CCF-A</option>
                <option value="5">中文CCF-B</option>
                <option value="6">无级别</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="paperRank">
              <Form.Label>排名</Form.Label>
              <Form.Control type="text" placeholder="请输入排名"/>
          </Form.Group>
          <Form.Group controlId="isCorrespondingAuthor">
              <Form.Label>是否通讯作者</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">是</option>
                <option value="0">否</option>
              </Form.Control>
          </Form.Group>
      </Form>
    );
}

const ModifyButton = (props) => {
    const { loadPapers } = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const [modifySN, setModifySN] = useState("");
    const [modifyTid, setModifyTid] = useState("");
    const [modifyPapers, setModifyPapers] = useState([]);

    const modify = async () => {
      try {
        const modifySNs = []
        const modifyTids = []
        const paperRanks = []
        const CorrespondingAuthors = []

        modifyPapers.forEach(element => {
          if(! modifySNs.includes(element.modifySN)) {
            modifySNs.push(element.modifySN);
            if(modifySNs.length > 1) {
              throw new Error("不支持同时修改涉及多篇论文的数据");
            }
          }

          if(! modifyTids.includes(element.modifyTid)) {
            modifyTids.push(element.modifyTid);
          }
          
          if(paperRanks.includes(element.paperRank)) {
            throw new Error("修改数据中排名存在重复");
            
          } 
          paperRanks.push(element.paperRank)
          if(parseInt(element.isCorrespondingAuthor) && !CorrespondingAuthors.includes(element.modifyTid)) {
            CorrespondingAuthors.push(element.modifyTid)
            if(CorrespondingAuthors.length > 1) {
              throw new Error("不允许一篇论文同时有多个通讯作者");
            }
          }
        })
        
        const body = {modifyPapers, modifyTids};
        const res = await fetch(`${HOST}:${PORT}/api/paper/admin`, {
          method: 'PATCH',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(body),
        });
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          closeModal();
          loadPapers();
        } else {
          alert(result.message);
        }
        setModifyPapers([]);
      } catch (err) {
        alert(err.message);
        setModifyPapers([]);
      }
    };
    const handleSave = async () => {
        const form = document.forms.modifyForm;
        const title = form.title.value;
        const publicSource = form.publicSource.value;
        const publicationYear = form.publicationYear.value;
        const publicType = form.publicType.value;
        const publicRank = form.publicRank.value;
        const paperRank = form.paperRank.value;
        const isCorrespondingAuthor = form.isCorrespondingAuthor.value;
        const body = { 
            modifySN, title, publicSource, publicationYear, publicType, publicRank, paperRank, isCorrespondingAuthor, modifyTid 
        };
        setModifyPapers([...modifyPapers, body]);
    }

    return (
      <div>
        <Button variant="outline-warning" size="sm" onClick={() => showModal()}>
                  修改
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
          <Modal.Header closeButton>
            <Modal.Title>修改论文</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <ModifyForm setModifySN={setModifySN} modifySN={modifySN} setModifyTid={setModifyTid} modifyTid={modifyTid}/>
          </Modal.Body>
          <Modal.Footer>
            <Button variant="secondary" onClick={() => closeModal()}>
                          关闭
            </Button>
            <Button variant="secondary" onClick={() => handleSave()}>
                          保存
            </Button>
            <Button variant="primary" onClick={() => modify()}>
                          提交
            </Button>
          </Modal.Footer>
        </Modal>
      </div>
  );
}

const DeleteForm = (props) => {
  const { deleteSN, setDeleteSN, deleteTid, setDeleteTid } = props; 
  const handleDeleteSNChange = (event) => {
    setDeleteSN(event.target.value);
  };

  const handleDeleteTidChange = (event) => {
    setDeleteTid(event.target.value);
};

  return (
    <Form id="deleteForm">
        <Form.Group controlId="id1">
            <Form.Label>论文号</Form.Label>
            <Form.Control type="text" placeholder="请输入待删除论文的论文号" value={deleteSN} onChange={handleDeleteSNChange} />
        </Form.Group>
        <Form.Group controlId="id2">
            <Form.Label>教师号</Form.Label>
            <Form.Control type="text" placeholder="请输入待删除论文作者的教师号" value={deleteTid} onChange={handleDeleteTidChange} />
        </Form.Group>
    </Form>
  );
}

const DeleteButton = (props) => {
    const { loadPapers } = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const [deleteSN, setDeleteSN] = useState("");
    const [deleteTid, setDeleteTid] = useState("");
    const [deletePapers, setDeletePapers] = useState([]);
    const handle = async () => {
      try {
        const deleteSNs = []
        deletePapers.forEach(element => {
          if(! deleteSNs.includes(element.deleteSN)) {
            deleteSNs.push(element.deleteSN);
            if(deleteSNs.length > 1) {
              throw new Error("不支持同时删除涉及多篇论文的数据");
            }
          }
        })

        const body = {deletePapers};
        const res = await fetch(
          `${HOST}:${PORT}/api/paper/admin`,{
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(body),
          },
        );
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          loadPapers(); 
        } else {
          alert(result.message);
        }
      } catch (err) {
        alert(err.message);
        
      }
      closeModal();
      setDeletePapers([]);
    };
    const handleDelete = async () => {
      if (confirm('确认删除这些课程吗？')) {
        handle();
      }
    };

    const handleSave = async () => {
        const body = { 
            deleteSN,  deleteTid 
        };
        setDeletePapers([...deletePapers, body]);
    }

    return (
      <div>
        <Button variant="outline-warning" size="sm" onClick={() => showModal()}>
                  删除
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
          <Modal.Header closeButton>
            <Modal.Title>删除论文</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <DeleteForm setDeleteSN={setDeleteSN} deleteSN={deleteSN} deleteTid={deleteTid} setDeleteTid={setDeleteTid}/>
          </Modal.Body>
          <Modal.Footer>
            <Button variant="secondary" onClick={() => closeModal()}>
                          关闭
            </Button>
            <Button variant="secondary" onClick={() => handleSave()}>
                          保存
            </Button>
            <Button variant="primary" onClick={() => handleDelete()}>
                          提交
            </Button>
          </Modal.Footer>
        </Modal>
      </div>
    );
}

const MyRow = (props) => {
    const { paper } = props;
    const publicTypeMap = {
      "": "无",
      "1": "full paper",
      "2": "short paper",
      "3": "poster paper",
      "4": "demo paper",
    }
    const publicRankMap = {
      "": "无",
      "1": "CCF-A",
      "2": "CCF-B",
      "3": "CCF-C",
      "4": "中文CCF-A",
      "5": "中文CCF-B",
      "6": "无级别",
    }
    const isCorrespondingAuthorMap = {
        true: "是",
        false: "否"
    }
    return (
      <tr>
        <td>{paper.sn}</td>
        <td>{paper.tid}</td>
        <td>{paper.title}</td>
        <td>{paper.publicSource}</td>
        <td>{paper.publicationYear}</td>
        <td>{publicTypeMap[paper.publicType]}</td>
        <td>{publicRankMap[paper.publicRank]}</td>
        <td>{paper.paperRank}</td>
        <td>{isCorrespondingAuthorMap[Boolean(paper.isCorrespondingAuthor)]}</td>
      </tr>
    );
  };

const PaperList = (props) => {
    const {loadPapers, papers} = props;
    useEffect(() => {loadPapers()}, []);
    const rows = papers.map((paper, idx) => (
      <MyRow key={idx} paper={paper} />
    ));
    return (
      <Table bordered hover>
        <thead>
          <tr>
            <th>论文号</th>
            <th>教师号</th>
            <th>论文名称</th>
            <th>发表源</th>
            <th>发表年份</th>
            <th>类型</th>
            <th>级别</th>
            <th>排名</th>
            <th>是否通讯作者</th>
          </tr>
        </thead>
        <tbody>
          {rows}
        </tbody>
      </Table>
    );
};

const PaperListTable = (props) => {
    const {papers, loadPapers} = props;
    const { auth } = useContext(teacherContext);
    
    return (
      <Container>
        {auth && (
          <Col md={12} className="d-flex justify-content-between align-items-center mb-3">
            <PostButton loadPapers={loadPapers} />
            <SearchButton loadPapers={loadPapers} />
            <ModifyButton loadPapers={loadPapers} />
            <DeleteButton loadPapers={loadPapers} />
          </Col>
        )}
        <PaperList loadPapers={loadPapers} papers={papers} />
      </Container>
    );
}

const SearchButton = (props) => {
  const {loadPapers} = props;
  const [show, setShow] = useState(false);
  const showModal = () => setShow(true);
  const closeModal = () => setShow(false);

  const handleSearch = async () => {
    const form = document.forms.searchForm;
    const tid = form.tid.value;
    const sn = form.sn.value;
    const title = form.title.value;
    const publicSource = form.publicSource.value;
    const publicationYear = form.publicationYear.value;
    const publicType = form.publicType.value;
    const publicRank = form.publicRank.value;
    const paperRank = form.paperRank.value;
    const isCorrespondingAuthor = form.isCorrespondingAuthor.value;

    const body = { 
      sn, title, publicSource, publicationYear, publicType, publicRank, paperRank, isCorrespondingAuthor, tid 
    };
    await loadPapers(body);
    closeModal();
  }

  const handleClear = async () => {
    const form = document.forms.searchForm;
    form.reset();
  }

  return (
    <div>
        <Button variant="outline-success" size="sm" onClick={() => showModal()}>
            搜索筛选
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
            <Modal.Header closeButton>
                <Modal.Title>搜索筛选</Modal.Title>
            </Modal.Header>
            <Modal.Body>
                <SearchForm/>
            </Modal.Body>
            <Modal.Footer>
                <Button variant="secondary" onClick={() => handleClear() }>
                    清除
                </Button>
                <Button variant="secondary" onClick={() => closeModal() }>
                    关闭
                </Button>
                <Button variant="primary" onClick={() => handleSearch()}>
                    搜索
                </Button>
            </Modal.Footer>
        </Modal>
    </div>
);

}

const SearchForm = () => {
  return (
    <Form id="searchForm">
      <Form.Group controlId="tid">
          <Form.Label>教师号</Form.Label>
          <Form.Control type="text" placeholder="请输入教师号"/>
      </Form.Group>
      <Form.Group controlId="sn">
        <Form.Label>论文号</Form.Label>
        <Form.Control type="text" placeholder="请输入论文号"/>
      </Form.Group>
      <Form.Group controlId="title">
          <Form.Label>论文名称</Form.Label>
          <Form.Control type="text" placeholder="请输入论文名称"/>
      </Form.Group>
      <Form.Group controlId="publicSource">
          <Form.Label>发表源</Form.Label>
          <Form.Control type="text" placeholder="请输入发表源"/>
      </Form.Group>
      <Form.Group controlId="publicationYear">
          <Form.Label>发表年份</Form.Label>
          <Form.Control type="text" placeholder="请输入发表年份"/>
      </Form.Group>
      <Form.Group controlId="publicType">
          <Form.Label>类型</Form.Label>
          <Form.Control as="select">
            <option value="">无</option>
            <option value="1">full paper</option>
            <option value="2">short paper</option>
            <option value="3">poster paper</option>
            <option value="4">demo paper</option>
          </Form.Control>
      </Form.Group>
      <Form.Group controlId="publicRank">
          <Form.Label>级别</Form.Label>
          <Form.Control as="select">
            <option value="">无</option>
            <option value="1">CCF-A</option>
            <option value="2">CCF-B</option>
            <option value="3">CCF-C</option>
            <option value="4">中文CCF-A</option>
            <option value="5">中文CCF-B</option>
            <option value="6">无级别</option>
          </Form.Control>
      </Form.Group>
      <Form.Group controlId="paperRank">
          <Form.Label>排名</Form.Label>
          <Form.Control type="text" placeholder="请输入排名"/>
      </Form.Group>
      <Form.Group controlId="isCorrespondingAuthor">
          <Form.Label>是否通讯作者</Form.Label>
          <Form.Control as="select">
            <option value="">无</option>
            <option value="1">是</option>
            <option value="0">否</option>
          </Form.Control>
      </Form.Group>
    </Form>
  );
}

const APaperListPage = () => {
  const [papers, setPapers] = useState([]);

  const loadPapers = async (body) => {
    try {      
      const res = await fetch(`${HOST}:${PORT}/api/paper/search`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
      });
      const result = await res.json();
      if (res.ok) {
        setPapers(result.data);
      } else {
        alert(result.message);
      }
    } catch (err) {
      alert(err.message);
    }
  };

  const styles = {
    form: {left: "50px",}
  }

  return (
    <div style={styles.form}>
       <PaperListTable papers = {papers} loadPapers = {loadPapers}/>
    </div>
  );
};

export default APaperListPage;