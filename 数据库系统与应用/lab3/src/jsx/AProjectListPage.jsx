import React, { useState, useEffect, useContext } from 'react';
import { Button, Container, Table, Modal, Form, Col } from 'react-bootstrap';

import { HOST, PORT, DOMAIN } from '../config';
import { teacherContext } from './App';

const PostForm = (props) => {

  const { postPid, setPostPid} = props;

  const handlePostPidChange = (event) => {
    setPostPid(event.target.value);
  };

  const handleButtonClick = async () => {
    try {
      const res = await fetch(`${HOST}:${PORT}/api/project/project/${postPid}`, { method: 'GET' });
      const result = await res.json();
      if (res.ok) {
        const form = document.forms.postForm;
        form.pid.value = result.data.pid;
        form.pname.value = result.data.pname;
        form.projectSource.value = result.data.projectSource;
        form.projectType.value = result.data.projectType;
        form.totalBudget.value = result.data.totalBudget;
        form.startYear.value = result.data.startYear;
        form.endYear.value = result.data.endYear;
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
            <Form.Label>待载入数据的项目号</Form.Label>
            <Form.Control type="text" placeholder="请输入待载入项目的项目号" value={postPid} onChange={handlePostPidChange} />
        </Form.Group>
        <Form.Group id="button">
            <Button variant="primary" className="ml-2" onClick={handleButtonClick}>载入</Button>
        </Form.Group>
          <Form.Group controlId="pid">
              <Form.Label>项目号</Form.Label>
              <Form.Control type="text" placeholder="请输入项目号"/>
          </Form.Group>
          <Form.Group controlId="pname">
              <Form.Label>项目名称</Form.Label>
              <Form.Control type="text" placeholder="请输入项目名称"/>
          </Form.Group>
          <Form.Group controlId="projectSource">
              <Form.Label>项目来源</Form.Label>
              <Form.Control type="text" placeholder="请输入发表源"/>
          </Form.Group>
          <Form.Group controlId="projectType">
              <Form.Label>项目类型</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">国家级项目</option>
                <option value="2">省部级项目</option>
                <option value="3">市厅级项目</option>
                <option value="4">企业合作项目</option>
                <option value="5">其他类型项目</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="totalBudget">
              <Form.Label>总经费</Form.Label>
              <Form.Control type="text" placeholder="请输入项目总经费"/>
          </Form.Group>
          <Form.Group controlId="startYear">
              <Form.Label>开始年份</Form.Label>
              <Form.Control type="text" placeholder="请输入开始年份"/>
          </Form.Group>
          <Form.Group controlId="endYear">
              <Form.Label>结束年份</Form.Label>
              <Form.Control type="text" placeholder="请输入结束年份"/>
          </Form.Group>
          <Form.Group controlId="tid">
              <Form.Label>教师号</Form.Label>
              <Form.Control type="text" placeholder="请输入教师号"/>
          </Form.Group>
          <Form.Group controlId="projectRank">
              <Form.Label>排名</Form.Label>
              <Form.Control type="text" placeholder="请输入排名"/>
          </Form.Group>
          <Form.Group controlId="undertakeFunding">
              <Form.Label>承担经费</Form.Label>
              <Form.Control type="text" placeholder="请输入承担经费"/>
          </Form.Group>
      </Form>
    );
}

const PostButton = (props) => {
    const {loadProjects} = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const [postPid, setPostPid] = useState("");
    const [postProjects, setPostProjects] = useState([]);
    const post = async () => {
        try {

            const postPids = []
            const projectRanks = [];
            let totalUndertakeFunding = 0;

            postProjects.forEach(element => {
                if(! postPids.includes(element.pid)) {
                    postPids.push(element.pid);
                    if(postPids.length > 1) {
                      throw new Error("不支持同时新增涉及多个项目的数据");
                    }
                }
                if(projectRanks.includes(element.projectRank)) {
                    throw new Error("新增数据中排名存在重复");
                } 
                totalUndertakeFunding += parseInt(element.undertakeFunding);
            })

            if(totalUndertakeFunding !== parseInt(postProjects[0].totalBudget)) {
                throw new Error("所有教师承担经费之和不等于项目总经费")
            }

            const body = {postProjects};

            const res = await fetch(`${HOST}:${PORT}/api/project/admin`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(body)
            }) ;
            const result = await res.json();
            if(res.ok) {
                alert(result.message);
                loadProjects();
                closeModal();
                setPostProjects([])
            } else {
                alert(result.message);
                closeModal();
                setPostProjects([])
            }
        } catch(err) {
            alert(err.message);
            closeModal();
            setPostProjects([])
        } 
    }

    const handleSave = async () => {
        const form = document.forms.postForm;
        const pid = form.pid.value;
        const tid = form.tid.value;
        const pname = form.pname.value;
        const projectSource = form.projectSource.value;
        const projectType = form.projectType.value;
        const totalBudget = form.totalBudget.value;
        const startYear = form.startYear.value;
        const endYear = form.endYear.value;
        const projectRank = form.projectRank.value;
        const undertakeFunding = form.undertakeFunding.value;
        const body = { 
            pid, pname, projectSource, projectType, totalBudget, startYear, endYear, projectRank, undertakeFunding, tid 
        };
        setPostProjects([...postProjects, body]);
    }

    return (
        <div>
            <Button variant="outline-success" size="sm" onClick={() => showModal()}>
                新建项目
            </Button>
            <Modal show={show} onHide={() => closeModal()}>
                <Modal.Header closeButton>
                    <Modal.Title>新建项目</Modal.Title>
                </Modal.Header>
                <Modal.Body>
                    <PostForm postPid={postPid} setPostPid={setPostPid}/>
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
    const { modifyPid, setModifyPid, modifyTid, setModifyTid } = props;

    const handleModifyPidChange = (event) => {
      setModifyPid(event.target.value);
    };

    const handleModifyTidChange = (event) => {
      setModifyTid(event.target.value);
    };

    const handleButtonClick = async () => {
      try {
        const res = await fetch(`${HOST}:${PORT}/api/project/${modifyTid}/${modifyPid}`, { method: 'GET' });
        const result = await res.json();
        if (res.ok) {
          const form = document.forms.modifyForm;
          form.pname.value = result.data.pname;
          form.projectSource.value = result.data.projectSource;
          form.projectType.value = result.data.projectType;
          form.totalBudget.value = result.data.totalBudget;
          form.startYear.value = result.data.startYear;
          form.endYear.value = result.data.endYear;
          form.projectRank.value = result.data.projectRank;
          form.undertakeFunding.value = result.data.undertakeFunding;
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
            <Form.Label>项目号</Form.Label>
            <Form.Control type="text" placeholder="请输入待修改项目的项目号" value={modifyPid} onChange={handleModifyPidChange} />
          </Form.Group>
          <Form.Group controlId="mid2">
            <Form.Label>教师号</Form.Label>
            <Form.Control type="text" placeholder="请输入待修改项目承担人的教师号" value={modifyTid} onChange={handleModifyTidChange} />
          </Form.Group>
          <Form.Group id="button">
            <Button variant="primary" className="ml-2" onClick={handleButtonClick}>载入</Button>
          </Form.Group>
          <Form.Group controlId="pname">
              <Form.Label>项目名称</Form.Label>
              <Form.Control type="text" placeholder="请输入项目名称"/>
          </Form.Group>
          <Form.Group controlId="projectSource">
              <Form.Label>项目来源</Form.Label>
              <Form.Control type="text" placeholder="请输入发表源"/>
          </Form.Group>
          <Form.Group controlId="projectType">
              <Form.Label>项目类型</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">国家级项目</option>
                <option value="2">省部级项目</option>
                <option value="3">市厅级项目</option>
                <option value="4">企业合作项目</option>
                <option value="5">其他类型项目</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="totalBudget">
              <Form.Label>总经费</Form.Label>
              <Form.Control type="text" placeholder="请输入项目总经费"/>
          </Form.Group>
          <Form.Group controlId="startYear">
              <Form.Label>开始年份</Form.Label>
              <Form.Control type="text" placeholder="请输入开始年份"/>
          </Form.Group>
          <Form.Group controlId="endYear">
              <Form.Label>开始年份</Form.Label>
              <Form.Control type="text" placeholder="请输入结束年份"/>
          </Form.Group>
          <Form.Group controlId="projectRank">
              <Form.Label>排名</Form.Label>
              <Form.Control type="text" placeholder="请输入排名"/>
          </Form.Group>
          <Form.Group controlId="undertakeFunding">
              <Form.Label>承担经费</Form.Label>
              <Form.Control type="text" placeholder="请输入承担经费"/>
          </Form.Group>
      </Form>
    );
}

const ModifyButton = (props) => {
    const { loadProjects } = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const [modifyPid, setModifyPid] = useState("");
    const [modifyTid, setModifyTid] = useState("");
    const [modifyProjects, setModifyProjects] = useState([]);

    const modify = async () => {
      try {

        const modifyPids = [];
        const modifyTids = []
        const projectRanks = [];

        modifyProjects.forEach(element => {
          if(! modifyPids.includes(element.modifyPid)) {
            modifyPids.push(element.modifyPid);
            if(modifyPids.length > 1) {
              throw new Error("不支持同时修改涉及多个项目的数据");
            }
          }
          if(! modifyTids.includes(element.modifyTid)) {
            modifyTids.push(element.modifyTid);
          }

          if(projectRanks.includes(element.projectRank)) {
              throw new Error("修改数据中排名存在重复");
          } 
          projectRanks.push(element.projectRank);
      })

        const body = {modifyProjects, modifyTids};

        const res = await fetch(`${HOST}:${PORT}/api/project/admin`, {
          method: 'PATCH',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(body),
        });
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          loadProjects();
          closeModal();
          setModifyProjects([])
        } else {
          alert(result.message);
          closeModal();
          setModifyProjects([])
        }
      } catch (err) {
        alert(err.message);
        closeModal();
        setModifyProjects([])
      }
    };

    const handleSave = async () => {
      const form = document.forms.modifyForm;
      const pname = form.pname.value;
      const projectSource = form.projectSource.value;
      const projectType = form.projectType.value;
      const totalBudget = form.totalBudget.value;
      const startYear = form.startYear.value;
      const endYear = form.endYear.value;
      const projectRank = form.projectRank.value;
      const undertakeFunding = form.undertakeFunding.value;
      const body = {
        modifyPid, pname, projectSource, projectType, totalBudget, startYear, endYear, projectRank, undertakeFunding, modifyTid
      }
      setModifyProjects([...modifyProjects, body]);
    }

    return (
      <div>
        <Button variant="outline-warning" size="sm" onClick={() => showModal()}>
                  修改
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
          <Modal.Header closeButton>
            <Modal.Title>修改项目</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <ModifyForm  setModifyPid={setModifyPid} modifyPid={modifyPid} setModifyTid={setModifyTid} modifyTid={modifyTid}/>
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
  const { deletePid, setDeletePid} = props; 

  const handleDeletePidChange = (event) => {
    setDeletePid(event.target.value);
  };
  return (
    <Form id="deleteForm">
        <Form.Group controlId="pid">
          <Form.Label>待删除项目的项目号</Form.Label>
            <Form.Control type="text" placeholder="请输入待删除项目的项目号" value={deletePid} onChange={handleDeletePidChange} />
        </Form.Group>
    </Form>
  );
}

const DeleteButton = (props) => {
    const { loadProjects } = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);

    const [deletePid, setDeletePid] = useState("");

    const handle = async (body) => {
      try {

        const res = await fetch(
          `${HOST}:${PORT}/api/project/admin`,{
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(body),
          },
        );
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          loadProjects();
          closeModal();
          setDeletePid("");
        } else {
          alert(result.message);
          closeModal();
        setDeletePid("");
        }
      } catch (err) {
        alert(err.message);
        closeModal();
        setDeletePid("");
      }
      
    };
    const handleDelete = async () => {
      if (confirm('确认删除该项目吗？')) {
        const body = { deletePid};
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
            <Modal.Title>删除项目</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <DeleteForm deletePid={deletePid} setDeletePid={setDeletePid}/>
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
    const { project } = props;
    const projectTypeMap = {
      "": "无",
      "1": "国家级项目",
      "2": "省部级项目",
      "3": "市厅级项目",
      "4": "企业合作项目",
      "5": "其他类型项目"
    }

    return (
      <tr>
        <td>{project.pid}</td>
        <td>{project.tid}</td>
        <td>{project.pname}</td>
        <td>{project.projectSource}</td>
        <td>{projectTypeMap[project.projectType]}</td>
        <td>{project.totalBudget}</td>
        <td>{project.startYear}</td>
        <td>{project.endYear}</td>
        <td>{project.projectRank}</td>
        <td>{project.undertakeFunding}</td>
      </tr>
    );
  };

const ProjectList = (props) => {
    const {loadProjects, projects} = props;
    useEffect(() => {loadProjects()}, []);
    const rows = projects.map((project, idx) => (
      <MyRow key={idx} project={project} />
    ));
    return (
      <Table bordered hover>
        <thead>
          <tr>
            <th>项目号</th>
            <th>教师号</th>
            <th>项目名称</th>
            <th>项目来源</th>
            <th>项目类型</th>
            <th>总经费</th>
            <th>开始年份</th>
            <th>结束年份</th>
            <th>排名</th>
            <th>承担经费</th>
          </tr>
        </thead>
        <tbody>
          {rows}
        </tbody>
      </Table>
    );
};

const ProjectListTable = (props) => {
    const {projects, loadProjects} = props;
    const { auth } = useContext(teacherContext);

    return (
      <Container>
        {auth && (
          <Col md={12} className="d-flex justify-content-between align-items-center mb-3">
            <PostButton loadProjects={loadProjects} />
            <SearchButton loadProjects={loadProjects} />
            <ModifyButton loadProjects={loadProjects} />
            <DeleteButton loadProjects={loadProjects} />
          </Col>
        )}
        <ProjectList loadProjects={loadProjects} projects={projects} />
      </Container>
    );
}

const SearchButton = (props) => {
  const {loadProjects} = props;
  const [show, setShow] = useState(false);
  const showModal = () => setShow(true);
  const closeModal = () => setShow(false);

  const handleSearch = async () => {
    const form = document.forms.searchForm;
    const tid = form.tid.value;
    const pid = form.pid.value;
    const pname = form.pname.value;
    const projectSource = form.projectSource.value;
    const projectType = form.projectType.value;
    const totalBudget = form.totalBudget.value;
    const startYear = form.startYear.value;
    const endYear = form.endYear.value;
    const projectRank = form.projectRank.value;
    const undertakeFunding = form.undertakeFunding.value;
    const body = { 
      pid, pname, projectSource, projectType, totalBudget, startYear, endYear, projectRank, undertakeFunding, tid 
    };
    await loadProjects(body);
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

const SearchForm = (props) => {
  return (
    <Form id="searchForm">
      <Form.Group controlId="tid">
          <Form.Label>教师号</Form.Label>
          <Form.Control type="text" placeholder="请输入教师号"/>
      </Form.Group>
      <Form.Group controlId="pid">
          <Form.Label>项目号</Form.Label>
          <Form.Control type="text" placeholder="请输入项目号"/>
      </Form.Group>
      <Form.Group controlId="pname">
          <Form.Label>项目名称</Form.Label>
          <Form.Control type="text" placeholder="请输入项目名称"/>
      </Form.Group>
      <Form.Group controlId="projectSource">
          <Form.Label>项目来源</Form.Label>
          <Form.Control type="text" placeholder="请输入发表源"/>
      </Form.Group>
      <Form.Group controlId="projectType">
          <Form.Label>项目类型</Form.Label>
          <Form.Control as="select">
            <option value="">无</option>
            <option value="1">国家级项目</option>
            <option value="2">省部级项目</option>
            <option value="3">市厅级项目</option>
            <option value="4">企业合作项目</option>
            <option value="5">其他类型项目</option>
          </Form.Control>
      </Form.Group>
      <Form.Group controlId="totalBudget">
          <Form.Label>总经费</Form.Label>
          <Form.Control type="text" placeholder="请输入项目总经费"/>
      </Form.Group>
      <Form.Group controlId="startYear">
          <Form.Label>开始年份</Form.Label>
          <Form.Control type="text" placeholder="请输入开始年份"/>
      </Form.Group>
      <Form.Group controlId="endYear">
          <Form.Label>开始年份</Form.Label>
          <Form.Control type="text" placeholder="请输入结束年份"/>
      </Form.Group>
      <Form.Group controlId="projectRank">
          <Form.Label>排名</Form.Label>
          <Form.Control type="text" placeholder="请输入排名"/>
      </Form.Group>
      <Form.Group controlId="undertakeFunding">
          <Form.Label>承担经费</Form.Label>
          <Form.Control type="text" placeholder="请输入承担经费"/>
      </Form.Group>
    </Form>
         
  );
}



const AProjectListPage = () => {
  const [projects, setProjects] = useState([]);
  const loadProjects= async (body) => {
    try {      
      const res = await fetch(`${HOST}:${PORT}/api/project/search`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
      });
      const result = await res.json();
      if (res.ok) {
        setProjects(result.data);
      } else {
        alert(result.message);
      }
    } catch (err) {
      alert(err.message);
    }
  };

  const styles = {
    form: {left: "50px",},
  }

  return (
    <div style={styles.form}>
        <ProjectListTable projects = {projects} loadProjects = {loadProjects}/>
    </div>
  );
};
export default AProjectListPage;