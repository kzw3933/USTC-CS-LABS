import React, { useState, useEffect, useContext } from 'react';
import { Button, Container, Table, Modal, Form, Col } from 'react-bootstrap';

import { HOST, PORT, DOMAIN } from '../config';
import { teacherContext } from './App';

const PostForm = () => {
    return (
      <Form id="postForm">
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

const PostButton = (props) => {
    const {loadPapers} = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const post = async (body) => {
        try {
            const res = await fetch(`${HOST}:${PORT}/api/paper`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(body)
            }) ;
            const result = await res.json();
            if(res.ok) {
                alert(result.message);
                closeModal();
                loadPapers();
            } else {
                alert(result.message);
            }
        } catch(err) {
            alert(err.message);
        } 
    }
    const handlePost = async () => {
        const {tid} = JSON.parse(await localStorage.getItem(DOMAIN));
        const form = document.forms.postForm;
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
        post(body);
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
                    <PostForm/>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="secondary" onClick={() => closeModal() }>
                        关闭
                    </Button>
                    <Button variant="primary" onClick={() => handlePost()}>
                        提交
                    </Button>
                </Modal.Footer>
            </Modal>
        </div>
    );
}

const ModifyForm = (props) => {
    const { modifySN, setModifySN } = props;
    const {tid} = JSON.parse(localStorage.getItem(DOMAIN));
    
    const handleModifySNChange = (event) => {
      setModifySN(event.target.value);
    };

    const handleButtonClick = async () => {
      try {
        const res = await fetch(`${HOST}:${PORT}/api/paper/${tid}/${modifySN}`, { method: 'GET' });
        const result = await res.json();
        if (res.ok) {
          const form = document.forms.modifyForm;
          form.sn.value = result.data.sn;
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
          <Form.Group controlId="sn">
            <Form.Label>待修改论文的论文号</Form.Label>
            <div className="d-flex">
              <Form.Control type="text" placeholder="请输入待修改论文的论文号" value={modifySN} onChange={handleModifySNChange} />
              <Button variant="primary" className="ml-2" onClick={handleButtonClick}>载入</Button>
            </div>
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
    const modify = async (body) => {
      try {
        const res = await fetch(`${HOST}:${PORT}/api/paper`, {
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
      } catch (err) {
        alert(err.message);
      }
    };
    const handleModify = async () => {
      const { tid } = JSON.parse(await localStorage.getItem(DOMAIN));
      const form = document.forms.modifyForm;
      const title = form.title.value;
      const publicSource = form.publicSource.value;
      const publicationYear = form.publicationYear.value;
      const publicType = form.publicType.value;
      const publicRank = form.publicRank.value;
      const paperRank = form.paperRank.value;
      const isCorrespondingAuthor = form.isCorrespondingAuthor.value;
      const body = { 
        modifySN, title, publicSource, publicationYear, publicType, publicRank, paperRank, isCorrespondingAuthor, tid 
      };
      modify(body);
    };
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
            <ModifyForm setModifySN={setModifySN} modifySN={modifySN}/>
          </Modal.Body>
          <Modal.Footer>
            <Button variant="secondary" onClick={() => closeModal()}>
                          关闭
            </Button>
            <Button variant="primary" onClick={() => handleModify()}>
                          提交
            </Button>
          </Modal.Footer>
        </Modal>
      </div>
  );
}

const DeleteForm = (props) => {
  const { deleteSN, setDeleteSN } = props; 
  const handleDeleteSNChange = (event) => {
    setDeleteSN(event.target.value);
  };

  return (
    <Form id="deleteForm">
        <Form.Group controlId="sn">
          <Form.Label>待删除论文的论文号</Form.Label>
            <Form.Control type="text" placeholder="请输入待删除论文的论文号" value={deleteSN} onChange={handleDeleteSNChange} />
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
    const handle = async (body) => {
      try {
        const res = await fetch(
          `${HOST}:${PORT}/api/paper`,{
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(body),
          },
        );
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          closeModal();
          loadPapers();
        } else {
          alert(result.message);
          loadPapers();
        }
      } catch (err) {
        alert(err.message);
      }
    };
    const handleDelete = async () => {
      if (confirm('确认删除该课程吗？')) {
        const { tid } = JSON.parse(localStorage.getItem(DOMAIN));
        const body = { deleteSN, tid};
        handle(body);
      }
    };
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
            <DeleteForm setDeleteSN={setDeleteSN} deleteSN={deleteSN}/>
          </Modal.Body>
          <Modal.Footer>
            <Button variant="secondary" onClick={() => closeModal()}>
                          关闭
            </Button>
            <Button variant="primary" onClick={() => handleDelete()}>
                          删除
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

const PaperListPage = () => {
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

export default PaperListPage;